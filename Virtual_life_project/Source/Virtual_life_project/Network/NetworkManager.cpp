#include "NetworkManager.h"

 // todo: 여기 정리..
RecvManager::RecvManager(FSocket* socket) 
    : Socket(socket)
{
}

RecvManager::~RecvManager()
{
}

bool RecvManager::Init()
{
	return false;
}

uint32 RecvManager::Run()
{
    constexpr int32 BufferSize = 1024;
    TArray<uint8> PacketBuffer;

    while (running && Socket && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
    {
        if (ReceivePacket(PacketBuffer))
        {
            // 받은 패킷 처리 (예: 로그 출력)
            FString ReceivedData = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(PacketBuffer.GetData())));
           // UE_LOG(LogTemp, Log, TEXT("패킷 수신: %s"), *ReceivedData);
        }
        else
        {
           // UE_LOG(LogTemp, Error, TEXT("패킷 수신 실패 또는 연결 종료"));
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

// 패킷 수신 (전체 패킷을 받아야 성공 반환)
bool RecvManager::ReceivePacket(TArray<uint8>& OutPacket)
{
    uint8 Header[HEADER_SIZE];

    // 헤더 먼저 읽기 (패킷 크기 확인)
    if (!ReceiveDesiredBytes(Header, HEADER_SIZE))
    {
        return false;
    }

    int32 PacketSize = *reinterpret_cast<int32*>(Header);
    if (PacketSize <= 0 || PacketSize > 4096)  // 기본적인 패킷 크기 검증
    {
        return false;
    }

    // 본문 읽기
    OutPacket.SetNumUninitialized(PacketSize);
    return ReceiveDesiredBytes(OutPacket.GetData(), PacketSize);
}

// 특정 크기의 데이터를 정확히 수신
bool RecvManager::ReceiveDesiredBytes(uint8* Results, int32 Size)
{
    int32 BytesRead = 0;
    int32 TotalBytesRead = 0;

    while (TotalBytesRead < Size)
    {
        if (!Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromMilliseconds(100)))
        {
            return false;  // 읽을 데이터 없음
        }

        if (!Socket->Recv(Results + TotalBytesRead, Size - TotalBytesRead, BytesRead))
        {
            return false;  // 수신 실패
        }

        TotalBytesRead += BytesRead;
    }

    return true;
}