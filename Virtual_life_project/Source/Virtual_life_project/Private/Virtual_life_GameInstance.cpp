// Fill out your copyright notice in the Description page of Project Settings.

// todo: 여기 해야 함

#include "Virtual_life_GameInstance.h"
#include "Sockets.h"
#include <Common/TcpSocketBuilder.h>
#include "m_CustomizableSkeletalComponent.h"
#include <Serialization/ArrayWriter.h>
#include "Networking.h"
#include "../Virtual_life_projectCharacter.h"
#include "SocketSubsystem.h"
#include "EngineUtils.h"
#include "VL_AnimInstance.h"
#include "VL_Player.h"
#include "FloatingTextWidget.h"


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

void UVirtual_life_GameInstance::SendGetItemPacket(uint8 item_id, uint8 num)
{
	CS_GET_ITEM_PACKET p;
	p.size = sizeof(CS_GET_ITEM_PACKET);
	p.type = CS_GET_ITEM;
	p.id = item_id;
	p.num = num;

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendEnterGamePacket()
{
	CS_ENTER_GAME_PACKET p;
	p.size = sizeof(CS_ENTER_GAME_PACKET);
	p.type = CS_ENTER_GAME;
	FCString::Strcpy(p.name, name.Len()+1, *name);
	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendUpadteCustomPacket()
{
	// todo: 커마 데이터 어떻게 가져옴?
	CS_UPDATE_CUSTOM_PACKET	p;
	p.size = sizeof(CS_UPDATE_CUSTOM_PACKET);
	p.type = CS_UPDATE_CUSTOM;
	custom_packet_setup(p.c, m_custom);
	SendEnqueue(&p, p.size);

}

void UVirtual_life_GameInstance::SendUpdateGoldPacket(int cs_gold_offset)
{
	CS_UPDATE_GOLD_PACKET p;
	p.size = sizeof(CS_UPDATE_GOLD_PACKET);
	p.type = CS_UPDATE_GOLD;
	p.gold_offset = cs_gold_offset;
	SendEnqueue(&p, p.size);
	OnGoldChanged.Broadcast(p.gold_offset);
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
	strcpy_s(p.id, M_ID_SIZE, TCHAR_TO_ANSI(*s));
	p.type = CS_LOGIN;
	

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SpawnPlayer()
{
	// 서버 연결 확인
	if (!Socket || !Socket->GetConnectionState() == SCS_Connected)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: Server connection is not established! Aborting SpawnPlayer."));
		return;
	}

	UWorld* World = GetWorld();

	// 1. 서버에서 받은 내 좌표로 나 이동.
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			auto p = Cast<AVL_Player>(PlayerPawn);
			p->isMyPlayer = true;
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
		FVector SpawnLocation(i.first.x, i.first.y, i.first.z);
		FRotator SpawnRotation(0.f, i.first.yaw, 0.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACharacter* Actor = World->SpawnActor<ACharacter>(
			PlayerClass, SpawnLocation, SpawnRotation, SpawnParams);

		Um_CustomizableSkeletalComponent* Other_actor_m_custom = Actor->FindComponentByClass<Um_CustomizableSkeletalComponent>();
		Other_actor_m_custom->custom_data_update(i.second);

		SpawnedPlayers.Add(i.first.id, Actor);
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

void UVirtual_life_GameInstance::custom_data_update(UCustom_data* targer_data, Customizing recv_cus)
{
	targer_data->skin = recv_cus.skin;
	targer_data->eye_scale = recv_cus.eye_scale;
	targer_data->hair = recv_cus.hair;
	targer_data->hair_color_B = recv_cus.hair_color_B;
	targer_data->hair_color_G = recv_cus.hair_color_G;
	targer_data->hair_color_R = recv_cus.hair_color_R;
	targer_data->L_eye_color_hue = recv_cus.L_eye_color_hue;
	targer_data->L_eye_color_sat = recv_cus.L_eye_color_sat;
	targer_data->R_eye_color_hue = recv_cus.R_eye_color_hue;
	targer_data->R_eye_color_sat = recv_cus.R_eye_color_sat;
	targer_data->pants = recv_cus.pants;
	targer_data->pupil_scale = recv_cus.pupil_scale;
	targer_data->shirt = recv_cus.shirt;
	targer_data->shoes = recv_cus.shoes;

	targer_data->eye_width = recv_cus.eye_width;
	targer_data->eye_thick = recv_cus.eye_thick;
	targer_data->eye_slope = recv_cus.eye_slope;
	targer_data->nose_width = recv_cus.nose_width;
	targer_data->nose_height = recv_cus.nose_height;
	targer_data->mouse_width = recv_cus.mouse_width;
	targer_data->mouse_thick = recv_cus.mouse_thick;
	targer_data->mouse_slope = recv_cus.mouse_slope;
	targer_data->chin = recv_cus.chin;
	targer_data->jaw = recv_cus.jaw;
	targer_data->heavy = recv_cus.heavy;
	targer_data->face_width = recv_cus.face_width;
}

void UVirtual_life_GameInstance::custom_packet_setup(Customizing& targer_data, const UCustom_data* recv_cus)
{
	targer_data.skin = recv_cus->skin;
	targer_data.eye_scale = recv_cus->eye_scale;
	targer_data.hair = recv_cus->hair;
	targer_data.hair_color_B = recv_cus->hair_color_B;
	targer_data.hair_color_G = recv_cus->hair_color_G;
	targer_data.hair_color_R = recv_cus->hair_color_R;
	targer_data.L_eye_color_hue = recv_cus->L_eye_color_hue;
	targer_data.L_eye_color_sat = recv_cus->L_eye_color_sat;
	targer_data.R_eye_color_hue = recv_cus->R_eye_color_hue;
	targer_data.R_eye_color_sat = recv_cus->R_eye_color_sat;
	targer_data.pants = recv_cus->pants;
	targer_data.pupil_scale = recv_cus->pupil_scale;
	targer_data.shirt = recv_cus->shirt;
	targer_data.shoes = recv_cus->shoes;

	targer_data.eye_width =   recv_cus->eye_width;
	targer_data.eye_thick =   recv_cus->eye_thick;
	targer_data.eye_slope =   recv_cus->eye_slope;
	targer_data.nose_width =  recv_cus->nose_width;
	targer_data.nose_height = recv_cus->nose_height;
	targer_data.mouse_width = recv_cus->mouse_width;
	targer_data.mouse_thick = recv_cus->mouse_thick;
	targer_data.mouse_slope = recv_cus->mouse_slope;
	targer_data.chin =        recv_cus->chin;
	targer_data.jaw =         recv_cus->jaw;
	targer_data.heavy =       recv_cus->heavy;
	targer_data.face_width =  recv_cus->face_width;
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
		case SC_LOGININFO: // 로그인 성공/실패
		{
			SC_LOGIN_INFO_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_LOGIN_INFO_PACKET));

			// 로그인 성공 -> 타이틀 맵으로 이동
			if (true == p.success) {
				UGameplayStatics::OpenLevel(this, FName("TitleMap"));
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Fail!")));
				// todo: leave 패킷 송신 필요
			}

			break;
		}
		case SC_ENTER_GAME:
		{
			SC_ENTER_GAME_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_ENTER_GAME_PACKET));

			name = p.name;

			// 위치 등 정보 추가
			MyPlayerInfo = p.player;

			// 직업 등 추가 정보
			AddInfo = p.addinfo;

			// 커마 데이터 넘겨주기
			custom_data_update(m_custom, p.custom);

			// todo: 퀘스트 데이터 넘겨주기
			for (int i = 0; i < QUEST_MAX; ++i) {
				if(p.num[i] != unsigned short(-1))
					m_quest->ADD_QUEST(p.num[i]);
			}


			// 메인 맵으로 이동
			UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("OpenWorldMap"))); // todo: 여기 수정
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Success!")));
			break;
		}
		case SC_SPAWN:
		{
			if (!loaded) {
				SC_SPAWN_PACKET p;
				FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_SPAWN_PACKET));
				NeedSpawnPoints.Add(std::make_pair(p.pl, p.c));
			}
			else {
				std::lock_guard<std::mutex> ll{ lock };
				SC_SPAWN_PACKET p;
				FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_SPAWN_PACKET));

				FVector L(p.pl.x, p.pl.y, p.pl.z);
				FRotator R(0.f, p.pl.yaw, 0.f);

				UWorld* World = GetWorld();

				// SpawnParams에 올바른 레벨 설정
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.OverrideLevel = World->PersistentLevel;

				ACharacter* Actor = World->SpawnActor<ACharacter>(PlayerClass, L, R, SpawnParams);

				//스폰된 액터의 커스텀정보 반영
				Um_CustomizableSkeletalComponent* Other_actor_m_custom = Actor->FindComponentByClass<Um_CustomizableSkeletalComponent>();
				Other_actor_m_custom->custom_data_update(p.c);
				// 스폰된 액터 저장
				SpawnedPlayers.Add(p.pl.id, Actor);

			}
			break;
		}
		case SC_DESPAWN:
		{
			SC_DESPAWN_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_DESPAWN_PACKET));
			if (auto FoundPlayer = SpawnedPlayers.Find(p.id))
			{
				auto PlayerActor = *FoundPlayer;
				if (IsValid(PlayerActor))
				{
					PlayerActor->Destroy();
					SpawnedPlayers.Remove(p.id);
				}
			}
			break;
		}

		case SC_CHAT:
		{
			SC_CHAT_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_CHAT_PACKET));

			FString Name = FString(p.name);
			FString Message = FString(p.msg);
			FString str = FString::Printf(TEXT("[ %s ]: %s"), *Name, *Message);

			OnChatReceived.Broadcast(str);

			break;
		}
		case SC_MOVEP:
		{
			if (!loaded) break;
			std::lock_guard<std::mutex> ll{ lock };
			SC_MOVE_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_MOVE_PACKET));

			ACharacter** FoundPlayer = SpawnedPlayers.Find(p.pl.id);
			ACharacter* PlayerActor = *FoundPlayer;

			FVector NewLocation(p.pl.x, p.pl.y, p.pl.z);
			FRotator NewRotation(0.f, p.pl.yaw, 0.f);

			auto pl = Cast<AVL_Player>(PlayerActor);
			pl->setDestInfo(p.pl);
			pl->setState(p.pl.st);

			

			break;
		}
		case SC_UPDATE_ITEM:
		{
			SC_UPDATE_ITEM_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_UPDATE_ITEM_PACKET));

			m_inventory->Add_Item(p.id, p.num);
			OnInventoryChanged.Broadcast(p.id);
			break;
		}
		case SC_UPDATE_GOLD:
		{
			SC_UPDATE_GOLD_PACKET	p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_UPDATE_GOLD_PACKET));
			AddInfo.gold = p.gold;
			OnGoldUpdated.Broadcast(AddInfo.gold);
			break;
		}
		}
	}
}

UVirtual_life_GameInstance::UVirtual_life_GameInstance()
{

	m_inventory = CreateDefaultSubobject<UPlayerInventory>(TEXT("PlayerInventory"));
	m_custom = CreateDefaultSubobject<UCustom_data>(TEXT("Custom_data"));
	m_quest = CreateDefaultSubobject<UQuest_Manager>(TEXT("Quest_Manager"));

	UE_LOG(LogTemp, Log, TEXT("Korean Text: %s"), *name);
}

int32 UVirtual_life_GameInstance::GetCurrentGold() const
{
	return AddInfo.gold;
}

void UVirtual_life_GameInstance::ShowFloatingText(const FString& Text, const FLinearColor& Color, const FVector& WorldLocation)
{
	UClass* BPClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("Blueprint'/Game/UI/FloatingText_W.FloatingText_W_C'"));
	if (BPClass)
	{
		UFloatingTextWidget* Widget = CreateWidget<UFloatingTextWidget>(GetWorld(), BPClass);
		if (Widget)
		{
			Widget->AddToViewport();
			Widget->SetTextAndPlay(Text, Color);

			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				FVector2D ScreenPosition;
				if (UGameplayStatics::ProjectWorldToScreen(PC, WorldLocation + FVector(0, 0, 100), ScreenPosition))
				{
					Widget->SetPositionInViewport(ScreenPosition, true);
				}
			}
		}
	}
}

void UVirtual_life_GameInstance::OnStart()
{
	Super::OnStart();
	// 블루프린트 클래스 로드 (정확한 경로 사용)
	PlayerClass = StaticLoadClass(ACharacter::StaticClass(), nullptr, TEXT("Blueprint'/Game/VirtualLife_Character/VL_metahuman.VL_metahuman_C'"));

	ConnectServer();
}

void UVirtual_life_GameInstance::Shutdown()
{
	Super::Shutdown();

	SendLeavePacket();
	//받아라!!!!!!!!! 1초준다
	float Timeout = 1.0f; // 최대 1초 대기
	float Elapsed = 0.0f;
	while (!SendPacketQueue.IsEmpty() && Elapsed < Timeout)
	{
		FPlatformProcess::Sleep(0.01f); // 10ms 대기
		Elapsed += 0.01f;
	}
	//난 뒤졋다.
	//그리고 이제 보낼것이없다
	SendPacketQueue.Empty(); // 큐를 즉시 비움

	while (!SendPacketQueue.IsEmpty());
	if (RecvThread)
	{
		RecvThread->Destroy();  // 스레드 종료
		delete RecvThread;
		RecvThread = nullptr;
	}

	if (SendThread)
	{
		SendThread->Destroy();  // 스레드 종료
		delete SendThread;
		SendThread = nullptr;
	}
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
	p.pl.st = MyPlayerInfo.st;

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

	wcsncpy(p.name, *name, M_ID_SIZE - 1);
	p.name[M_ID_SIZE - 1] = '\0'; // Null-termination 보장

	// 메시지 설정
	wcsncpy(p.msg, *s, CHAT_SIZE - 1);
	p.msg[CHAT_SIZE - 1] = L'\0'; // Null-termination 보장

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendLeavePacket()
{
	CS_LEAVE_PACKET	p;
	p.size = sizeof(CS_LEAVE_PACKET);
	p.type = CS_LEAVE;

	SendEnqueue(&p, p.size);
}
