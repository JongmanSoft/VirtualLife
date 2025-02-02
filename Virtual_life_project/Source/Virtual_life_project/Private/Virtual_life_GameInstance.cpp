// Fill out your copyright notice in the Description page of Project Settings.

// todo: 여기 해야 함

#include "Virtual_life_GameInstance.h"
#include "Sockets.h"
#include <Common/TcpSocketBuilder.h>
#include <Serialization/ArrayWriter.h>
#include "Networking.h"
#include "../Virtual_life_projectCharacter.h"
#include "SocketSubsystem.h"

void UVirtual_life_GameInstance::ConnectServer()
{
	// 소켓 생성
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	// ip주소 넘겨주기.
	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port); // 내가 붙어야 하는 서버.

	// 디버깅용
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	// connected가 true이면 연결 성공.
	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected) { // 연결 성공
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// 스레드 동작 시작하기
		RecvThread = new RecvManager(Socket, this);
		SendThread = new SendManager(Socket, this);

	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

// 패킷을 송신 큐에 추가하는 함수
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

	// 큐에 있는 모든 패킷 처리
	while (RecvPacketQueue.Dequeue(PacketData))
	{
		// 패킷 ID 추출
		if (PacketData.Num() < HEADER_SIZE) continue;

		uint16 PacketSize;
		uint8 PacketType;

		FMemory::Memcpy(&PacketSize, PacketData.GetData(), sizeof(uint16));
		FMemory::Memcpy(&PacketType, PacketData.GetData() + sizeof(uint16), sizeof(uint8));

		// 패킷 종류에 따라 처리
		switch (PacketType)
		{
		case SC_LOGININFO:

			SC_LOGIN_INFO_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_LOGIN_INFO_PACKET));

			// 로그인 성공: 메인 맵으로 이동
			if (true == p.success) {
				MyPlayerInfo = p.player;
				UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("NewMap")));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Success!")));
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Fail!")));
				// todo: leave 패킷 송신 필요
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

	// 레벨이 로드될 때 호출
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UVirtual_life_GameInstance::OnLevelLoaded);
}

void UVirtual_life_GameInstance::OnLevelLoaded(UWorld* LoadedWorld)
{
	if (LoadedWorld->GetMapName().Contains("NewMap"))  // MainMap인지 확인
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(LoadedWorld, 0);
		if (PlayerController)
		{
			APawn* PlayerPawn = PlayerController->GetPawn();
			if (PlayerPawn)
			{
				// 서버로부터 받은 위치 정보 적용
				FVector NewLocation(MyPlayerInfo.x, MyPlayerInfo.y, MyPlayerInfo.z);
				FRotator NewRotation(0.f, MyPlayerInfo.yaw, 0.f);

				PlayerPawn->SetActorLocationAndRotation(NewLocation, NewRotation);
				UE_LOG(LogTemp, Log, TEXT("플레이어 위치 조정 완료 : (% f, % f, % f)"), NewLocation.X, NewLocation.Y, NewLocation.Z);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("PlayerPawn이 존재하지 않습니다."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerController를 찾을 수 없습니다."));
		}
	}
}
