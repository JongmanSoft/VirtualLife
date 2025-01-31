#include "NetworkManager.h"

 // todo: 여기 정리..
RecvManager::RecvManager(FSocket* socket, UVirtual_life_GameInstance* ins)
    : Socket(socket), Instance(ins)
{
    Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
}

RecvManager::~RecvManager()
{
    Destroy();
}

bool RecvManager::Init()
{
    UE_LOG(LogTemp, Log, TEXT("RecvManager Initialized"));
    return true;
}

uint32 RecvManager::Run()
{
    while (running)
    {
        if (!ReceivePacket())
        {
            UE_LOG(LogTemp, Error, TEXT("Packet receive failed"));
            running = false;
        }
    }
    return 0;
}

void RecvManager::Exit()
{
}

void RecvManager::Destroy()
{
    running = false;
    if (Socket)
    {
        Socket->Close();
        Socket = nullptr;
    }
    if (Thread)
    {
        Thread->Kill(true);
        delete Thread;
        Thread = nullptr;
    }
}

bool RecvManager::ReceivePacket()
{
    uint8 TempBuffer[1024];  // 한 번에 받을 임시 버퍼
    int32 BytesRead = 0;

    // 데이터를 계속 수신
    if (Socket->Recv(TempBuffer, sizeof(TempBuffer), BytesRead))
    {
        if (BytesRead > 0)
        {
            Buffer.Append(TempBuffer, BytesRead);  //수신된 데이터를 내부 버퍼에 추가

            // 버퍼에서 완전한 패킷을 추출할 수 있을 때까지 반복
            while (ProcessPacket());
            return true;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Socket Recv Failed"));
        running = false;
    }

    return false;
}

// **버퍼에서 완성된 패킷을 추출하여 큐에 저장**
bool RecvManager::ProcessPacket()
{
    if (Buffer.Num() < HEADER_SIZE)
    {
        return false;  // 헤더 크기만큼 데이터가 없으면 대기
    }

    uint16 PacketSize = *reinterpret_cast<uint16*>(Buffer.GetData());  // 패킷 크기 읽기
    if (PacketSize < HEADER_SIZE || PacketSize > 4096)  // 유효성 검사
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid packet size received: %d"), PacketSize);
        Buffer.RemoveAt(0, HEADER_SIZE);  // 유효하지 않은 헤더 삭제 후 다음 데이터 확인
        return false;
    }

    if (Buffer.Num() < PacketSize)
    {
        return false;  // 전체 패킷이 도착하지 않았으면 대기
    }

    // 완성된 패킷을 추출하여 큐에 저장
    TArray<uint8> CompletedPacket;
    CompletedPacket.Append(Buffer.GetData(), PacketSize);

    if (Instance)
    {
        Instance->RecvPacketQueue.Enqueue(CompletedPacket);
    }

    // 사용한 데이터 삭제 (남은 데이터는 다음 패킷 조립에 사용)
    Buffer.RemoveAt(0, PacketSize);

    return true;
}

SendWorker::SendWorker(FSocket* socket, UVirtual_life_GameInstance* ins)
    : Socket(socket), Instance(ins), Running(true)
{
    Thread = FRunnableThread::Create(this, TEXT("SendThread"));
}

SendWorker::~SendWorker()
{
    Destroy();
}

bool SendWorker::Init()
{
    UE_LOG(LogTemp, Log, TEXT("SendWorker Initialized"));
    return true;
}

uint32 SendWorker::Run()
{
    while (Running)
    {
        // 큐에서 패킷을 가져와 전송
        TArray<uint8> Packet;
        if (Instance->SendPacketQueue.Dequeue(Packet))
        {
            if (!SendDesiredBytes(Packet.GetData(), Packet.Num()))
            {
                UE_LOG(LogTemp, Error, TEXT("Packet sending failed"));
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Packet sent successfully, Size: %d"), Packet.Num());
            }
        }

        FPlatformProcess::Sleep(0.01f); // CPU 점유율 감소
    }
    return 0;
}

void SendWorker::Exit()
{
    Running = false;
}

void SendWorker::Destroy()
{
    Running = false;
    if (Thread)
    {
        Thread->Kill(true);
        delete Thread;
        Thread = nullptr;
    }
}

// 패킷을 송신 큐에 추가하는 함수
bool SendWorker::SendPacket(void* packet, int32 PacketSize)
{
    TArray<uint8> PacketData;
    PacketData.Append(reinterpret_cast<uint8*>(packet), PacketSize);
    
    Instance->SendPacketQueue.Enqueue(PacketData);
    return true;
}

// 특정 크기의 데이터를 정확히 송신
bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{
    int32 BytesSent = 0;
    int32 TotalBytesSent = 0;

    while (TotalBytesSent < Size)
    {
        int32 Sent = 0;
        if (!Socket->Send(Buffer + TotalBytesSent, Size - TotalBytesSent, Sent) || Sent <= 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Socket Send Failed"));
            return false;
        }
        TotalBytesSent += Sent;
    }
    return true;
}