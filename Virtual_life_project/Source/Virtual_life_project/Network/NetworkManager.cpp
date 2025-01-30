#include "NetworkManager.h"

 // todo: ���� ����..
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
            // ���� ��Ŷ ó�� (��: �α� ���)
            FString ReceivedData = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(PacketBuffer.GetData())));
           // UE_LOG(LogTemp, Log, TEXT("��Ŷ ����: %s"), *ReceivedData);
        }
        else
        {
           // UE_LOG(LogTemp, Error, TEXT("��Ŷ ���� ���� �Ǵ� ���� ����"));
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

// ��Ŷ ���� (��ü ��Ŷ�� �޾ƾ� ���� ��ȯ)
bool RecvManager::ReceivePacket(TArray<uint8>& OutPacket)
{
    uint8 Header[HEADER_SIZE];

    // ��� ���� �б� (��Ŷ ũ�� Ȯ��)
    if (!ReceiveDesiredBytes(Header, HEADER_SIZE))
    {
        return false;
    }

    int32 PacketSize = *reinterpret_cast<int32*>(Header);
    if (PacketSize <= 0 || PacketSize > 4096)  // �⺻���� ��Ŷ ũ�� ����
    {
        return false;
    }

    // ���� �б�
    OutPacket.SetNumUninitialized(PacketSize);
    return ReceiveDesiredBytes(OutPacket.GetData(), PacketSize);
}

// Ư�� ũ���� �����͸� ��Ȯ�� ����
bool RecvManager::ReceiveDesiredBytes(uint8* Results, int32 Size)
{
    int32 BytesRead = 0;
    int32 TotalBytesRead = 0;

    while (TotalBytesRead < Size)
    {
        if (!Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromMilliseconds(100)))
        {
            return false;  // ���� ������ ����
        }

        if (!Socket->Recv(Results + TotalBytesRead, Size - TotalBytesRead, BytesRead))
        {
            return false;  // ���� ����
        }

        TotalBytesRead += BytesRead;
    }

    return true;
}