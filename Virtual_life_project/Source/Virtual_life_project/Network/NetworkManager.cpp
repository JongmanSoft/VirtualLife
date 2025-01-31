#include "NetworkManager.h"

 // todo: ���� ����..
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
    uint8 TempBuffer[1024];  // �� ���� ���� �ӽ� ����
    int32 BytesRead = 0;

    // �����͸� ��� ����
    if (Socket->Recv(TempBuffer, sizeof(TempBuffer), BytesRead))
    {
        if (BytesRead > 0)
        {
            Buffer.Append(TempBuffer, BytesRead);  //���ŵ� �����͸� ���� ���ۿ� �߰�

            // ���ۿ��� ������ ��Ŷ�� ������ �� ���� ������ �ݺ�
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

// **���ۿ��� �ϼ��� ��Ŷ�� �����Ͽ� ť�� ����**
bool RecvManager::ProcessPacket()
{
    if (Buffer.Num() < HEADER_SIZE)
    {
        return false;  // ��� ũ�⸸ŭ �����Ͱ� ������ ���
    }

    uint16 PacketSize = *reinterpret_cast<uint16*>(Buffer.GetData());  // ��Ŷ ũ�� �б�
    if (PacketSize < HEADER_SIZE || PacketSize > 4096)  // ��ȿ�� �˻�
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid packet size received: %d"), PacketSize);
        Buffer.RemoveAt(0, HEADER_SIZE);  // ��ȿ���� ���� ��� ���� �� ���� ������ Ȯ��
        return false;
    }

    if (Buffer.Num() < PacketSize)
    {
        return false;  // ��ü ��Ŷ�� �������� �ʾ����� ���
    }

    // �ϼ��� ��Ŷ�� �����Ͽ� ť�� ����
    TArray<uint8> CompletedPacket;
    CompletedPacket.Append(Buffer.GetData(), PacketSize);

    if (Instance)
    {
        Instance->RecvPacketQueue.Enqueue(CompletedPacket);
    }

    // ����� ������ ���� (���� �����ʹ� ���� ��Ŷ ������ ���)
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
        // ť���� ��Ŷ�� ������ ����
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

        FPlatformProcess::Sleep(0.01f); // CPU ������ ����
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

// ��Ŷ�� �۽� ť�� �߰��ϴ� �Լ�
bool SendWorker::SendPacket(void* packet, int32 PacketSize)
{
    TArray<uint8> PacketData;
    PacketData.Append(reinterpret_cast<uint8*>(packet), PacketSize);
    
    Instance->SendPacketQueue.Enqueue(PacketData);
    return true;
}

// Ư�� ũ���� �����͸� ��Ȯ�� �۽�
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