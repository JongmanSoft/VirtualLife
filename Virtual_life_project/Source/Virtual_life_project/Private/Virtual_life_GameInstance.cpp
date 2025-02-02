// Fill out your copyright notice in the Description page of Project Settings.

// todo: ���� �ؾ� ��

#include "Virtual_life_GameInstance.h"
#include "Sockets.h"
#include <Common/TcpSocketBuilder.h>
#include <Serialization/ArrayWriter.h>
#include "Networking.h"
#include "../Virtual_life_projectCharacter.h"
#include "SocketSubsystem.h"

void UVirtual_life_GameInstance::ConnectServer()
{
	// ���� ����
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	// ip�ּ� �Ѱ��ֱ�.
	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port); // ���� �پ�� �ϴ� ����.

	// ������
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	// connected�� true�̸� ���� ����.
	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected) { // ���� ����
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// ������ ���� �����ϱ�
		RecvThread = new RecvManager(Socket, this);
		SendThread = new SendManager(Socket, this);

	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

// ��Ŷ�� �۽� ť�� �߰��ϴ� �Լ�
bool UVirtual_life_GameInstance::SendEnqueue(void* packet, int32 PacketSize)
{
	TArray<uint8> PacketData;
	PacketData.Append(reinterpret_cast<uint8*>(packet), PacketSize);

	SendPacketQueue.Enqueue(PacketData);
	return true;
}

void UVirtual_life_GameInstance::SendLoginInfoPacket(FString s)
{
	CS_LOGIN_PACKET	p;
	p.size = sizeof(CS_LOGIN_PACKET);
	strcpy_s(p.name, M_NAME_SIZE, TCHAR_TO_ANSI(*s));
	p.type = CS_LOGIN;

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::DisconnectServer()
{
}

void UVirtual_life_GameInstance::Tick(float DeltaTime)
{
	ProcessRecvPackets();
}

void UVirtual_life_GameInstance::ProcessRecvPackets()
{
	TArray<uint8> PacketData;

	// ť�� �ִ� ��� ��Ŷ ó��
	while (RecvPacketQueue.Dequeue(PacketData))
	{
		// ��Ŷ ID ����
		if (PacketData.Num() < HEADER_SIZE) continue;

		uint16 PacketSize;
		uint8 PacketType;

		FMemory::Memcpy(&PacketSize, PacketData.GetData(), sizeof(uint16));
		FMemory::Memcpy(&PacketType, PacketData.GetData() + sizeof(uint16), sizeof(uint8));

		// ��Ŷ ������ ���� ó��
		switch (PacketType)
		{
		case SC_LOGININFO:

			SC_LOGIN_INFO_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_LOGIN_INFO_PACKET));

			// �α��� ����: ���� ������ �̵�
			if (true == p.success) {
				MyPlayerInfo = p.player;
				UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("NewMap")));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Success!")));
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Fail!")));
				// todo: leave ��Ŷ �۽� �ʿ�
			}

			break;

		case SC_CHAT:
			break;

		default:
			UE_LOG(LogTemp, Warning, TEXT("Unknown Packet Type: %d"), PacketType);
			break;
		}
	}
}

void UVirtual_life_GameInstance::OnStart()
{
	Super::OnStart();

	// ������ �ε�� �� ȣ��
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UVirtual_life_GameInstance::OnLevelLoaded);
}

void UVirtual_life_GameInstance::OnLevelLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld->GetMapName().Contains("NewMap"))  // MainMap���� Ȯ��
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(LoadedWorld, 0);
		if (PlayerController)
		{
			APawn* PlayerPawn = PlayerController->GetPawn();
			if (PlayerPawn)
			{
				// �����κ��� ���� ��ġ ���� ����
				FVector NewLocation(MyPlayerInfo.x, MyPlayerInfo.y, MyPlayerInfo.z);
				FRotator NewRotation(0.f, MyPlayerInfo.yaw, 0.f);

				PlayerPawn->SetActorLocationAndRotation(NewLocation, NewRotation);
				UE_LOG(LogTemp, Log, TEXT("�÷��̾� ��ġ ���� �Ϸ� : (% f, % f, % f)"), NewLocation.X, NewLocation.Y, NewLocation.Z);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerPawn�� �������� �ʽ��ϴ�."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerController�� ã�� �� �����ϴ�."));
		}
	}
}
