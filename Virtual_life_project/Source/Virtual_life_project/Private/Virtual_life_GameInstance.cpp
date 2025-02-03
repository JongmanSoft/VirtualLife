// Fill out your copyright notice in the Description page of Project Settings.

// todo: 여기 해야 함

#include "Virtual_life_GameInstance.h"
#include "Sockets.h"
#include <Common/TcpSocketBuilder.h>
#include <Serialization/ArrayWriter.h>
#include "Networking.h"
#include "../Virtual_life_projectCharacter.h"
#include "SocketSubsystem.h"
#include "EngineUtils.h"


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
	name = s;

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SpawnPlayer()
{
	UWorld* World = GetWorld();

	// todo: world를 제한할지 고밍

	// 1. 서버에서 받은 내 좌표로 나 이동.
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			FVector NewLocation(MyPlayerInfo.x, MyPlayerInfo.y, MyPlayerInfo.z);
			FRotator NewRotation(0.f, MyPlayerInfo.yaw, 0.f);

			PlayerPawn->SetActorHiddenInGame(false);
			PlayerPawn->SetActorEnableCollision(true);
			PlayerPawn->SetActorTickEnabled(true);

			PlayerPawn->SetActorLocationAndRotation(NewLocation, NewRotation);
		}
	}

	// 2. 서버에서 받은 애들 스폰
	for (auto& i : NeedSpawnPoints) {
		FVector SpawnLocation(i.x, i.y, i.z);
		FRotator SpawnRotation(0.f, i.yaw, 0.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AVirtual_life_projectCharacter* Actor = World->SpawnActor<AVirtual_life_projectCharacter>(
			PlayerClass, SpawnLocation, SpawnRotation, SpawnParams);

		SpawnedPlayers.Add(i.id, Actor);
	}

	loaded = true;
}

void UVirtual_life_GameInstance::DisconnectServer()
{
}

void UVirtual_life_GameInstance::Tick(float DeltaTime)
{
	ProcessRecvPackets();

	if (true == loaded) {
		TimeAccumulator += DeltaTime;
		if (TimeAccumulator >= 0.1f)  
		{
			SendPlayerLocationToServer();
			TimeAccumulator = 0.0f;
		}
	}
}

void UVirtual_life_GameInstance::ProcessRecvPackets()
{
	TArray<uint8> PacketData;

	// 큐에 있는 모든 패킷 처리
	while (RecvPacketQueue.Dequeue(PacketData))
	{
		uint16 PacketSize;
		uint8 PacketType;

		FMemory::Memcpy(&PacketSize, PacketData.GetData(), sizeof(uint16));
		FMemory::Memcpy(&PacketType, PacketData.GetData() + sizeof(uint16), sizeof(uint8));

		UE_LOG(LogTemp, Log, TEXT(" Received Packet - Type: %d, Size: %d"), PacketType, PacketSize);

		// 패킷 종류에 따라 처리
		switch (PacketType)
		{
		case SC_LOGININFO:
		{
			SC_LOGIN_INFO_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_LOGIN_INFO_PACKET));

			// 로그인 성공: 메인 맵으로 이동
			if (true == p.success) {
				MyPlayerInfo = p.player;
				UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("testMap")));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Success!")));
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Fail!")));
				// todo: leave 패킷 송신 필요
			}

			break;
		}

		case SC_SPAWN:
		{
			if (!loaded) {
				SC_SPAWN_PACKET p;
				FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_SPAWN_PACKET));
				NeedSpawnPoints.Add(p.pl);
			}
			else {
				std::lock_guard<std::mutex> ll{ lock };
				SC_SPAWN_PACKET p;
				FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_SPAWN_PACKET));

				FVector L(p.pl.x, p.pl.y, 200.f);
				FRotator R(0.f, p.pl.yaw, 0.f);

				UWorld* World = GetWorld();

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AVirtual_life_projectCharacter* Actor = World->SpawnActor<AVirtual_life_projectCharacter>(
					PlayerClass, L, R, SpawnParams);

				// 스폰된 액터 저장
				SpawnedPlayers.Add(p.pl.id, Actor);
			}
			break;
		}
		case SC_CHAT:
			break;
		case SC_MOVEP:
		{
			if (!loaded) break;
			std::lock_guard<std::mutex> ll{ lock };
			SC_MOVE_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_MOVE_PACKET));

			AVirtual_life_projectCharacter** FoundPlayer = SpawnedPlayers.Find(p.pl.id);
			if (!FoundPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player ID %d not found in SpawnedPlayers."), p.pl.id);
				break;
			}

			AVirtual_life_projectCharacter* PlayerActor = *FoundPlayer;

			if (!IsValid(PlayerActor))
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid PlayerActor for ID %d. Removing from map."), p.pl.id);
				SpawnedPlayers.Remove(p.pl.id);  // 잘못된 참조 삭제
				break;
			}

			FVector NewLocation(p.pl.x, p.pl.y, p.pl.z);
			FRotator NewRotation(0.f, p.pl.yaw, 0.f);

			PlayerActor->SetActorLocationAndRotation(NewLocation, NewRotation);

			UE_LOG(LogTemp, Log, TEXT("Player %d moved to (%.2f, %.2f, %.2f)"), p.pl.id, NewLocation.X, NewLocation.Y, NewLocation.Z);

			break;
		}
		}
	}
}

void UVirtual_life_GameInstance::OnStart()
{
	Super::OnStart();

	// 블루프린트 클래스 로드 (정확한 경로 사용)
	PlayerClass = LoadClass<AVirtual_life_projectCharacter>(nullptr, TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UVirtual_life_GameInstance::OnLevelLoaded);
}

void UVirtual_life_GameInstance::OnLevelLoaded(UWorld* LoadedWorld)
{
}

void UVirtual_life_GameInstance::SendPlayerLocationToServer()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return;

	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn) return;

	FVector Location = PlayerPawn->GetActorLocation();
	FRotator Rotation = PlayerPawn->GetActorRotation();

	CS_MOVE_PACKET p;
	p.size = sizeof(CS_MOVE_PACKET);
	p.type = CS_MOVEP;
	p.pl.x = Location.X;
	p.pl.y = Location.Y;
	p.pl.z = Location.Z;
	p.pl.yaw = Rotation.Yaw;
	p.pl.id = MyPlayerInfo.id;

	if (true == loaded)  // SendThread가 존재할 경우 전송
	{
		SendEnqueue(&p, p.size);
	}

}

void UVirtual_life_GameInstance::SendChatPacket(FString s)
{
	CS_CHAT_PACKET p;
	p.size = sizeof(CS_CHAT_PACKET);
	p.type = CS_CHAT;

	FTCHARToUTF8 NameConverter(*name);
	strncpy(p.name, NameConverter.Get(), M_NAME_SIZE - 1);
	p.name[M_NAME_SIZE - 1] = '\0'; // Null-termination 보장

	// 메시지 설정
	wcsncpy(p.msg, *s, CHAT_SIZE - 1);
	p.msg[CHAT_SIZE - 1] = L'\0'; // Null-termination 보장

	SendEnqueue(&p, p.size);
}
