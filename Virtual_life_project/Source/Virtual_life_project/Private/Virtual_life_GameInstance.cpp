// Fill out your copyright notice in the Description page of Project Settings.

// todo: 여기 해야 함

#include "Virtual_life_GameInstance.h"
#include "Sockets.h"
#include <Common/TcpSocketBuilder.h>
#include "../Custom/m_CustomizableSkeletalComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Serialization/ArrayWriter.h>
#include "Networking.h"
#include "../Virtual_life_projectCharacter.h"
#include "SocketSubsystem.h"
#include "EngineUtils.h"
#include "VL_AnimInstance.h"
#include "VL_Player.h"
#include "FloatingTextWidget.h"
#include "../building/BuildItemRegistry.h"
#include "../building/FBuildInfo.h"
#include "../building/PlaceBuildActor.h"
#include "../building/InteractableActor.h"
#include "Engine/DataTable.h"
#include "NoticeFriendUIWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"


void UVirtual_life_GameInstance::ConnectServer(FString addr)
{
	// 소켓 생성
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));
	Socket->SetNoDelay(true);
	// ip주소 넘겨주기.
	FIPv4Address Ip;
	FIPv4Address::Parse(addr, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port); // 내가 붙어야 하는 서버.

	// 디버깅용
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	// connected가 true이면 연결 성공.
	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected) { // 연결 성공
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		RecvThread = new RecvManager(Socket, this);
		SendThread = new SendManager(Socket, this);

		LoginToVivox();
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UVirtual_life_GameInstance::SendGetItemPacket(uint8 item_id, int num)
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

void UVirtual_life_GameInstance::SendGetQuestPacket(uint8 giver_id, uint8 quest_id)
{
	CS_UPDATE_QUEST_PACKET p;
	p.size = sizeof(CS_UPDATE_GOLD_PACKET);
	p.type = CS_GET_QUEST;
	p.giver_id = giver_id;
	p.num = quest_id;
	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendRemoveQuestPacket(uint8 giver_id, uint8 quest_id)
{
	CS_UPDATE_QUEST_PACKET p;
	p.size = sizeof(CS_UPDATE_GOLD_PACKET);
	p.type = CS_REMOVE_QUEST;
	p.giver_id = giver_id;
	p.num = quest_id;
	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendPlaceBuildPacket(const TArray<FObjectData>& Objects)
{
	for (const FObjectData& Obj : Objects)
	{
		CS_PLACE_BUILD_PACKET p;
		p.size = sizeof(CS_PLACE_BUILD_PACKET);
		p.type = CS_PLACE_BUILD;
		p.build.item_id = static_cast<uint16>(Obj.ItemID);
		p.build.x = Obj.Location.X;
		p.build.y = Obj.Location.Y;
		p.build.z = Obj.Location.Z;
		p.build.yaw = Obj.Yaw;
		p.build.scale = Obj.Scale;

		SendEnqueue(&p, p.size);
	}

	// UE_LOG(LogTemp, Log, TEXT("총 %d개의 건물을 서버로 전송했습니다."), Objects.Num());
}

void UVirtual_life_GameInstance::SendRemoveBuildPacket(const FVector& Location)
{
	CS_REMOVE_BUILD_PACKET p;
	p.size = sizeof(CS_REMOVE_BUILD_PACKET);
	p.type = CS_REMOVE_BUILD;
	p.x = Location.X;
	p.y = Location.Y;
	p.z = Location.Z;

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendUpdateBuildPacket(const FVector& OldLoc, const FVector& NewLoc, float NewYaw)
{
	CS_UPDATE_BUILD_PACKET p;
	p.size = sizeof(CS_UPDATE_BUILD_PACKET);
	p.type = CS_UPDATE_BUILD;
	p.old_x = OldLoc.X;
	p.old_y = OldLoc.Y;
	p.old_z = OldLoc.Z;
	p.new_x = NewLoc.X;
	p.new_y = NewLoc.Y;
	p.new_z = NewLoc.Z;
	p.new_yaw = NewYaw;

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendRoomLeavePacket()
{
	bReturnFromRoom = true;

	CS_ROOM_LEAVE_PACKET p;
	p.size = sizeof(CS_ROOM_LEAVE_PACKET);
	p.type = CS_ROOM_LEAVE;

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendPartyUpdatePacket(const FString& Id_str)
{
	CS_UPDATE_PARTY_PACKET p;
	p.size = sizeof(CS_UPDATE_PARTY_PACKET);
	p.type = CS_UPDATE_PARTY;
	p.act_type = PARTY_REQUEST::PARTY_REQUEST_INVITE;
	strcpy_s(p.id, M_ID_SIZE, TCHAR_TO_ANSI(*Id_str));
	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendPartyJoinPacket(const FString& Id_str)
{
	CS_UPDATE_PARTY_PACKET p;
	p.size = sizeof(CS_UPDATE_PARTY_PACKET);
	p.type = CS_UPDATE_PARTY;
	p.act_type = PARTY_REQUEST::PARTY_REQUEST_INVITE_ACCEPT;
	strcpy_s(p.id, M_ID_SIZE, TCHAR_TO_ANSI(*Id_str));
	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendPartyRejectPacket(const FString& Id_str)
{
	CS_UPDATE_PARTY_PACKET p;
	p.size = sizeof(CS_UPDATE_PARTY_PACKET);
	p.type = CS_UPDATE_PARTY;
	p.act_type = PARTY_REQUEST::PARTY_REQUEST_INVITE_REJECT;
	strcpy_s(p.id, M_ID_SIZE, TCHAR_TO_ANSI(*Id_str));
	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendDoorStatePacket(const uint8& door_id, bool is_open)
{
	CS_UPDATE_DOOR_PACKET p;
	p.size = sizeof(CS_UPDATE_DOOR_PACKET);
	p.type = CS_DOOR_UPDATE;
	p.door_id = door_id;
	p.is_open = is_open;
	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendCreateKidPacket(unsigned int preg_id, unsigned int spouse_id, Customizing c, float x, float y, float z, float yaw, char personality, const FString& kid_name, const FString& kid_hello)
{
	CS_ADD_KID_PACKET p;
	p.size = sizeof(CS_ADD_KID_PACKET);
	p.type = CS_ADD_KID;
	p.preg_id = preg_id;
	p.spouse_id = spouse_id;
	p.c = c;
	p.x = x, p.y = y, p.z = z, p.yaw = yaw;
	p.personality = personality;
	FCString::Strcpy(p.name, kid_name.Len() + 1, *kid_name);
	FCString::Strcpy(p.hello_msg, kid_hello.Len() + 1, *kid_hello);
	SendEnqueue(&p, p.size);
}

bool UVirtual_life_GameInstance::SendEnqueue(void* packet, int32 PacketSize)
{
	TArray<uint8> PacketData;
	PacketData.Append(reinterpret_cast<uint8*>(packet), PacketSize);

	SendPacketQueue.Enqueue(PacketData);
	return true;
}

void UVirtual_life_GameInstance::ResetPlayers()
{
	loaded = false;

	for (auto& Pair : OtherPlayers)
	{
		ACharacter* Character = Pair.Value.character;

		if (IsValid(Character))
		{
			Character->Destroy();        // Actor 제거
		}
	}

	OtherPlayers.Empty();

	// 5. 로딩 플래그 초기화
}

void UVirtual_life_GameInstance::SendLoginInfoPacket(FString s, FString pw)
{
	CS_LOGIN_PACKET	p;
	p.size = sizeof(CS_LOGIN_PACKET);
	strcpy_s(p.id, M_ID_SIZE, TCHAR_TO_ANSI(*s));
	strcpy_s(p.pw, M_ID_SIZE, TCHAR_TO_ANSI(*pw));
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
	if (bReturnFromRoom)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
		if (PlayerController)
		{
			APawn* PlayerPawn = PlayerController->GetPawn();
			if (PlayerPawn)
			{
				auto p = Cast<AVL_Player>(PlayerPawn);
				p->isMyPlayer = true;

				PlayerPawn->SetActorHiddenInGame(false);
				PlayerPawn->SetActorEnableCollision(true);
				PlayerPawn->SetActorTickEnabled(true);

				PlayerPawn->SetActorLocationAndRotation(LastMainMapLocation, LastMainMapRotation);

				bReturnFromRoom = false; // 위치 복구 후 플래그 리셋
			}
		}
	}
	else
	{
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

				p->set_my_id(MyPlayerInfo.id);
			}
		}
	}

	// 2. 서버에서 받은 애들 스폰
	for (TPair<int, SpawnInfo>& Pair : OtherPlayers) {
		int PlayerID = Pair.Key;
		SpawnInfo& Info = Pair.Value;

		FVector SpawnLocation(Info.pinfo.x, Info.pinfo.y, Info.pinfo.z);
		FRotator SpawnRotation(0.f, Info.pinfo.yaw, 0.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACharacter* Actor = World->SpawnActor<ACharacter>(
			PlayerClass, SpawnLocation, SpawnRotation, SpawnParams);

		Um_CustomizableSkeletalComponent* Other_actor_m_custom = Actor->FindComponentByClass<Um_CustomizableSkeletalComponent>();
		Other_actor_m_custom->custom_data_update(Info.cinfo);

		Info.character = Actor;

		auto pl = Cast<AVL_Player>(Info.character);
		if (pl != nullptr) {
			pl->set_my_id(Info.pinfo.id);
		}
	}

	// 3. NPC 스폰
	for (auto& Pair : OtherNPCs) // TMap<uint32, NPCUnitData>
	{
		const NPCUnitData& npcData = Pair.Value.data;

		FVector Location(npcData.x, npcData.y, npcData.z);
		FRotator Rotation(0.f, npcData.yaw, 0.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACharacter* SpawnedNPC = World->SpawnActor<ACharacter>(PlayerClass, Location, Rotation, SpawnParams);
		Um_CustomizableSkeletalComponent* Other_actor_m_custom = SpawnedNPC->FindComponentByClass<Um_CustomizableSkeletalComponent>();
		Other_actor_m_custom->custom_data_update(npcData.c);

		Pair.Value.character = SpawnedNPC; // 스폰된 NPC를 TMap에 저장
	}

	// 문
	for (int i = 0; i < 13; i++) {
		UpdateDoor.Broadcast(current_door_id[i], current_is_open[i]);
	}

	loaded = true;
}

void UVirtual_life_GameInstance::SpawnPlayerInRoom()
{
	// 서버 연결 확인
	if (!Socket || !Socket->GetConnectionState() == SCS_Connected)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: Server connection is not established! Aborting SpawnPlayer."));
		return;
	}

	UWorld* World = GetWorld();

	// 2. 서버에서 받은 애들 스폰
	for (TPair<int, SpawnInfo>& Pair : OtherPlayers) {
		int PlayerID = Pair.Key;
		SpawnInfo& Info = Pair.Value;

		FVector SpawnLocation(Info.pinfo.x, Info.pinfo.y, Info.pinfo.z);
		FRotator SpawnRotation(0.f, Info.pinfo.yaw, 0.f);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACharacter* Actor = World->SpawnActor<ACharacter>(
			PlayerClass, SpawnLocation, SpawnRotation, SpawnParams);

		Um_CustomizableSkeletalComponent* Other_actor_m_custom = Actor->FindComponentByClass<Um_CustomizableSkeletalComponent>();
		Other_actor_m_custom->custom_data_update(Info.cinfo);

		Info.character = Actor;

		auto pl = Cast<AVL_Player>(Info.character);
		if (pl != nullptr) {
			pl->set_my_id(Info.pinfo.id);
		}
	}
	

	loaded = true;
}

void UVirtual_life_GameInstance::DisconnectServer()
{
}

void UVirtual_life_GameInstance::Tick(float DeltaTime)
{
	ProcessRecvPackets();

	if (GEngine)
	{
		FString Status = loaded ? TEXT("loaded: true") : TEXT("loaded: false");
		GEngine->AddOnScreenDebugMessage(
			/*Key*/ 1,           // 고유 ID (같은 키면 덮어씀)
			/*TimeToDisplay*/ 0.f, // 다음 프레임에도 계속 보이게
			/*Color*/ FColor::Yellow,
			/*Message*/ Status
		);
	}

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

	targer_data->eyebrows = recv_cus.eyebrows;
	targer_data->glasses = recv_cus.glasses;
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

	targer_data.eyebrows = recv_cus->eyebrows;
	targer_data.glasses = recv_cus->glasses;
}

ACharacter* UVirtual_life_GameInstance::draw_one_player(int draw_id)
{
	
	int PlayerID = draw_id;
	SpawnInfo& Info = OtherPlayers[PlayerID];

	FVector SpawnLocation(0, -240, 92);
	FRotator SpawnRotation(0.f, 0.f, -180.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UWorld* World = GetWorld();
	ACharacter* Actor = World->SpawnActor<ACharacter>(
		PlayerClass, SpawnLocation, SpawnRotation, SpawnParams);

	Um_CustomizableSkeletalComponent* Other_actor_m_custom = Actor->FindComponentByClass<Um_CustomizableSkeletalComponent>();
	Other_actor_m_custom->custom_data_update(Info.cinfo);

	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn player actor!"));
		return nullptr;
	}
	return Actor;
}

void UVirtual_life_GameInstance::PlayBGM(USoundCue* BGMSoundCue)
{
	if (nullptr != BGMaudio)BGMaudio = UGameplayStatics::SpawnSound2D(GetWorld(), BGMSoundCue,1,1,0.0,NULL,true,false);
}

void UVirtual_life_GameInstance::StopBGM()
{
	if(nullptr != BGMaudio)BGMaudio->SetActive(false);
}

int UVirtual_life_GameInstance::get_my_player_id()
{
	return MyPlayerInfo.id;
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

		// 패킷 종류에 따라 처리
		switch (PacketType)
		{
		case SC_LOGININFO: // 로그인 성공/실패
		{
			SC_LOGIN_INFO_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_LOGIN_INFO_PACKET));

			// 로그인 성공 -> 타이틀 맵으로 이동
			if (true == p.success) {
				if (p.is_new == false)
					SendEnterGamePacket();
				else
				{
					UGameplayStatics::OpenLevel(this, FName("RealTitleMap"));
				}
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Fail!")));
				// todo: leave 패킷 송신 필요
				SendPacketQueue.Empty(); // 큐 비우기
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

			// 커마 데이터 넘겨주기
			custom_data_update(m_custom, p.custom);

			// todo: 퀘스트 데이터 넘겨주기
			for (int i = 0; i < QUEST_MAX; ++i) {
				if(p.num[i] != signed short(-1))
					m_quest->ADD_QUEST(p.num[i]);
			}

			enter_time = p.time;

			// 메인 맵으로 이동
			UGameplayStatics::OpenLevel(GetWorld(), (TEXT("OpenWorldMap"))); 
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Success!")));
			StopBGM();

			break;
		}
		case SC_SPAWN:
		{
			if (!loaded) {
				SC_SPAWN_PACKET p;
				FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_SPAWN_PACKET));
				SpawnInfo ts{};
				ts.character = nullptr;
				ts.cinfo = p.c;
				ts.pinfo = p.pl;
				OtherPlayers.Add(p.pl.id, ts);
			}
			else {
				std::lock_guard<std::mutex> ll{ lock };
				SC_SPAWN_PACKET p;
				FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_SPAWN_PACKET));

				SpawnInfo ts{};
				ts.cinfo = p.c;
				ts.pinfo = p.pl;
				ts.character = nullptr;
				OtherPlayers.Add(p.pl.id, ts);

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
				OtherPlayers[p.pl.id].character = Actor;

				auto FoundPlayer = OtherPlayers.Find(p.pl.id);
				ACharacter* PlayerActor = FoundPlayer->character;
				auto pl = Cast<AVL_Player>(PlayerActor);
				if (pl != nullptr) {
					pl->set_my_id(p.pl.id);
				}
			}
			break;
		}
		case SC_DESPAWN:
		{
			SC_DESPAWN_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_DESPAWN_PACKET));
			if (auto FoundPlayer = OtherPlayers.Find(p.id))
			{
				auto PlayerActor = FoundPlayer;
				if (IsValid(PlayerActor->character))
				{
					PlayerActor->character->Destroy();
					OtherPlayers.Remove(p.id);
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
			FString strWithnotName = FString::Printf(TEXT("%s"), *Message);

			OnChatReceived.Broadcast(str);
			OnChatWithID.Broadcast(p.from_id, strWithnotName);

			break;
		}
		case SC_MOVEP:
		{
			if (!loaded) break;
			std::lock_guard<std::mutex> ll{ lock };
			SC_MOVE_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_MOVE_PACKET));

			auto FoundPlayer = OtherPlayers.Find(p.pl.id);

			if (FoundPlayer == nullptr || !IsValid(FoundPlayer->character))
				break;

			ACharacter* PlayerActor = FoundPlayer->character;
			if (PlayerActor == nullptr) break;

			FVector NewLocation(p.pl.x, p.pl.y, p.pl.z);
			FRotator NewRotation(0.f, p.pl.yaw, 0.f);

			auto pl = Cast<AVL_Player>(PlayerActor);
			if (pl != nullptr) {
				pl->setDestInfo(p.pl);
				pl->setState(p.pl.st);
			}
			break;
		}
		case SC_UPDATE_ITEM:
		{
			SC_UPDATE_ITEM_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_UPDATE_ITEM_PACKET));

			m_inventory->update_item(p.id, p.num);
			OnInventoryChanged.Broadcast(p.id);
			break;
		}
		case SC_UPDATE_GOLD:
		{
			SC_UPDATE_GOLD_PACKET	p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_UPDATE_GOLD_PACKET));
			MyPlayerInfo.gold = p.gold;
			OnGoldUpdated.Broadcast(MyPlayerInfo.gold);
			break;
		}
		case SC_GET_QUEST:
		{
			SC_UPDATE_QUEST_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_UPDATE_QUEST_PACKET));
			m_quest->ADD_QUEST(p.num);
			UE_LOG(LogTemp, Log, TEXT("퀘스트 추가"));
			OnQusetUpdate.Broadcast();
			break;
		}
		case SC_REMOVE_QUEST:
		{
			SC_UPDATE_QUEST_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_UPDATE_QUEST_PACKET));
			m_quest->Delete_Quest(p.num);
			OnQusetUpdate.Broadcast();
			break;
		}
		case SC_DOOR_UPDATE: {
			SC_UPDATE_DOOR_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_UPDATE_DOOR_PACKET));
			UpdateDoor.Broadcast(p.door_id,p.is_open);
			break;
		}
		case SC_DOORS_UPDATE: {
			SC_UPDATE_DOORS_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_UPDATE_DOORS_PACKET));
			constexpr int Door_num = 13;
			for (int num = 0; num < Door_num; num++) {
				current_door_id[num] = p.door_id[num] ;
			    current_is_open[num] = p.is_open[num];

				UE_LOG(LogTemp, Log, TEXT("%d번문 : %d"),
				current_door_id[num],current_is_open[num]);
			}
			break;
		}
		case SC_ROOM_SETUP:
		{
			SC_ROOM_SETUP_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_ROOM_SETUP_PACKET));

			HandleRoomSetup(p);

			break;
		}
		case SC_ROOM_LEAVE:
		{
			SC_ROOM_LEAVE_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_ROOM_LEAVE_PACKET));

			ResetPlayers();

			enter_time = p.time;
			UGameplayStatics::OpenLevel(this, FName(TEXT("OpenWorldMap")));
			break;
		}
		case SC_NPCS_SPAWN:
		{
			const uint8* raw = PacketData.GetData();
			const SC_SPAWN_NPCS_PACKET* p = reinterpret_cast<const SC_SPAWN_NPCS_PACKET*>(raw);

			// npc 배열은 패킷 뒤에 붙어 있음
			const NPCUnitData* npcs = reinterpret_cast<const NPCUnitData*>(raw + sizeof(SC_SPAWN_NPCS_PACKET));

			for (int i = 0; i < p->npc_count; ++i)
			{
				const NPCUnitData& unit = npcs[i];

				NPCInfo npc;
				npc.data = unit;
				npc.character = nullptr;

				OtherNPCs.Add(unit.id, npc);
			}
			break;
		}
		case SC_RESULT_PARTY:
		{
			SC_RESULT_PARTY_PACKET p;
			FMemory::Memcpy(&p, PacketData.GetData(), sizeof(SC_RESULT_PARTY_PACKET));

			if (p.act_type == PARTY_REQUEST::PARTY_REQUEST_INVITE) // 초대가 왔을 때
			{
				UWorld* World = GetWorld();
				if (World)
				{
					APlayerController* PC = World->GetFirstPlayerController();
					if (PC && NoticeWidgetClass)
					{
						UNoticeFriendUIWidget* Widget = CreateWidget<UNoticeFriendUIWidget>(PC, NoticeWidgetClass);
						if (Widget)
						{
							Widget->FriendID = FString(p.id);
							Widget->SetText(FString(p.name));
							Widget->AddToViewport();
						}
					}
				}
			}
			else if (p.act_type == PARTY_REQUEST::PARTY_REQUEST_INVITE_REJECT) // 상대방이 초대 거절
			{
				// todo: 여기 알림 메시지 추가
				int k = 0;
			}
			else if (p.act_type == PARTY_REQUEST::PARTY_REQUEST_INVITE_ACCEPT) // 상대방이 파티에 참여
			{
				// todo: 여기 알림 메시지 추가
				int k = 0;
			}
			else if (p.act_type == PARTY_REQUEST::PARTY_JOIN_SUCCESS) // 파티 참가 성공 -> 음성채팅 참가
			{
				FString str = p.channel_id;
				JoinChannel(str);
			}
		}
		}
	}
}

UVirtual_life_GameInstance::UVirtual_life_GameInstance()
{

	m_inventory = CreateDefaultSubobject<UPlayerInventory>(TEXT("PlayerInventory"));
	m_custom = CreateDefaultSubobject<UCustom_data>(TEXT("Custom_data"));
	m_quest = CreateDefaultSubobject<UQuest_Manager>(TEXT("Quest_Manager"));
	m_marry = CreateDefaultSubobject<Umarry_manager>(TEXT("Marry_Manager"));

}

int32 UVirtual_life_GameInstance::GetCurrentGold() const
{
	return MyPlayerInfo.gold;
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

	// 위젯 클래스 로드
	NoticeWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("Blueprint'/Game/UI/Friend_UI/NoticeFriend_UI.NoticeFriend_UI_C'"));

	UDataTable* BuildTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("DataTable'/Game/BuildingSystem/NewBuildData.NewBuildData'")));

	if (BuildTable)
	{
		FBuildItemRegistry::Initialize(BuildTable);
		BuildingDataTable = BuildTable;

		PlaceBuildClass = StaticLoadClass(APlaceBuildActor::StaticClass(), nullptr,TEXT("Blueprint'/Game/BuildingSystem/MyPlaceBuildActor.MyPlaceBuildActor_C'"));
	}
}

void UVirtual_life_GameInstance::SendEnterRoom(FString roomID)
{
	// 현재 플레이어 위치 저장
	if (ACharacter* MyChar = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		LastMainMapLocation = MyChar->GetActorLocation();
		LastMainMapRotation = MyChar->GetActorRotation();
	}

	// 초기화.
	ResetPlayers();

	CS_ROOM_ENTER_PACKET p;
	p.size = sizeof(CS_ROOM_ENTER_PACKET);
	strcpy_s(p.id, M_ID_SIZE, TCHAR_TO_ANSI(*roomID));
	p.type = CS_ROOM_ENTER;

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::SendEnterMyRoom()
{
	loaded = false;

	CS_ROOM_ENTER_PACKET p;
	p.size = sizeof(CS_ROOM_ENTER_PACKET);
	strcpy_s(p.id, M_ID_SIZE, TCHAR_TO_ANSI(*StrID));
	p.type = CS_ROOM_ENTER;

	SendEnqueue(&p, p.size);
}

void UVirtual_life_GameInstance::HandleRoomSetup(const SC_ROOM_SETUP_PACKET& p)
{
	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] SC_ROOM_SETUP_PACKET 수신!오브젝트 수 : % d"), p.count);

	CachedRoomObjects.Empty();

	// 받은 오브젝트 저장
	for (int i = 0; i < p.count; ++i)
	{
		CachedRoomObjects.Add(p.objs[i]);
		UE_LOG(LogTemp, Log, TEXT("Object[%d] - ID: %d, Pos: (%.1f, %.1f, %.1f), Yaw: %.1f"),
			i, p.objs[i].item_id, p.objs[i].x, p.objs[i].y, p.objs[i].z, p.objs[i].yaw);
	}

	UGameplayStatics::OpenLevel(this, MyRoomMapName);
}

void UVirtual_life_GameInstance::SpawnCachedRoomObjects()
{
	if (!PlaceBuildClass)
	{
		PlaceBuildClass = StaticLoadClass(APlaceBuildActor::StaticClass(), nullptr, TEXT("Blueprint'/Game/BuildingSystem/MyPlaceBuildActor.MyPlaceBuildActor_C'"));
		if (!PlaceBuildClass)
			return;
	}

	UWorld* World = GetWorld();
	if (!World)
		return;

	for (const Object& obj : CachedRoomObjects)
	{
		FName RowName = FBuildItemRegistry::ItemIDToFName(obj.item_id);
		if (RowName.IsNone()) continue;

		const FBuildInfo* Info = BuildingDataTable->FindRow<FBuildInfo>(RowName, TEXT(""));
		if (!Info) continue;

		FVector SpawnLoc(obj.x, obj.y, obj.z);
		FRotator SpawnRot(0.f, obj.yaw, 0.f);
		FVector SpawnScale = FVector(obj.scale);
		FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);

		if (Info->InteractableActorClass)
		{
			AInteractableActor* SpawnedActor = World->SpawnActor<AInteractableActor>(Info->InteractableActorClass, SpawnLoc, SpawnRot);
			if (SpawnedActor)
			{
				SpawnedActor->SetActorScale3D(DefaultScale);
				SpawnedActor->SetRowID(RowName);

				if (Info->Mesh)
				{
					if (UStaticMeshComponent* MeshComp = SpawnedActor->FindComponentByClass<UStaticMeshComponent>())
					{
						MeshComp->SetStaticMesh(Info->Mesh);
					}
				}
			}
		}
		else if (Info->Mesh)
		{
			APlaceBuildActor* SpawnedActor = World->SpawnActor<APlaceBuildActor>(PlaceBuildClass, SpawnLoc, SpawnRot);
			if (SpawnedActor)
			{
				SpawnedActor->SetMesh(Info->Mesh);
				SpawnedActor->SetRowID(RowName);
				SpawnedActor->SetScale(obj.scale, Info->bIsWall);
			}
		}
	}
}

void UVirtual_life_GameInstance::SpawnRoomObjectsFromData(const TArray<FObjectData>& ObjectList)
{
	if (!PlaceBuildClass)
	{
		PlaceBuildClass = StaticLoadClass(APlaceBuildActor::StaticClass(), nullptr,
			TEXT("Blueprint'/Game/BuildingSystem/MyPlaceBuildActor.MyPlaceBuildActor_C'"));

		if (!PlaceBuildClass) return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	for (const FObjectData& Obj : ObjectList)
	{
		FName RowName = FBuildItemRegistry::ItemIDToFName(Obj.ItemID);
		const FBuildInfo* Info = BuildingDataTable->FindRow<FBuildInfo>(RowName, TEXT(""));

		if (!Info || !Info->Mesh) continue;

		FVector Loc = Obj.Location;
		FRotator Rot(0.f, Obj.Yaw, 0.f);
		FVector SpawnScale = FVector(Obj.Scale);

		if (Info->InteractableActorClass)
		{
			AInteractableActor* SpawnedActor = World->SpawnActor<AInteractableActor>(Info->InteractableActorClass, Loc, Rot);
			if (SpawnedActor)
			{
				SpawnedActor->SetActorScale3D(SpawnScale);
				SpawnedActor->SetRowID(RowName);

				if (Info->Mesh)
				{
					if (UStaticMeshComponent* MeshComp = SpawnedActor->FindComponentByClass<UStaticMeshComponent>())
					{
						MeshComp->SetStaticMesh(Info->Mesh);
					}
				}
			}
		}
		else if (Info->Mesh)
		{
			APlaceBuildActor* Spawned_2 = World->SpawnActor<APlaceBuildActor>(PlaceBuildClass, Loc, Rot);
			if (Spawned_2)
			{
				Spawned_2->SetMesh(Info->Mesh);
				Spawned_2->SetRowID(RowName);

				if (Info->bIsWall)
					Spawned_2->SetScale(Obj.Scale, true);
				else
					Spawned_2->SetScale(Obj.Scale);
			}
		}
	}
}

void UVirtual_life_GameInstance::Shutdown()
{
	Super::Shutdown();

	// 음성채팅 비활
	if (bLoggedIn && VivoxClient)
	{
		ILoginSession& LoginSession = VivoxClient->GetLoginSession(LoggedInAccountID);
		if (LoginSession.State() == LoginState::LoggedIn)
		{
			LoginSession.Logout();
		}
	}

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

	p.from_id = MyPlayerInfo.id;

	wcsncpy(p.name, *name, M_ID_SIZE - 1);
	p.name[M_ID_SIZE - 1] = '\0'; // Null-termination 보장

	// 메시지 설정
	wcsncpy(p.msg, *s, CHAT_SIZE - 1);
	p.msg[CHAT_SIZE - 1] = L'\0'; // Null-termination 보장

	SendEnqueue(&p, p.size);
	OnChatWithID.Broadcast(p.from_id, s);
}

void UVirtual_life_GameInstance::SendLeavePacket()
{
	CS_LEAVE_PACKET	p;
	p.size = sizeof(CS_LEAVE_PACKET);
	p.type = CS_LEAVE;

	SendEnqueue(&p, p.size);
}


// -------------------------

// todo: 여기 키 다시 받아야 함.
#define VIVOX_VOICE_KEY TEXT("EuohA4cqIZGjCcaVHDX9T5qqZhfNeTkX")
#define VIVOX_VOICE_SERVER TEXT("https://unity.vivox.com/appconfig/14569-1029-42694-udash")
#define VIVOX_VOICE_DOMAIN TEXT("mtu1xp.vivox.com")
#define VIVOX_VOICE_ISSUER TEXT("14569-1029-42694-udash")

struct FVivoxToken
{
	static void GenerateClientLoginToken(const ILoginSession& LoginSession, FString& OutToken);
	static void GenerateClientJoinToken(const IChannelSession& ChannelSession, FString& OutToken);
};

void FVivoxToken::GenerateClientLoginToken(const ILoginSession& LoginSession, FString& OutToken)
{
	FTimespan TokenExpiration = FTimespan::FromSeconds(90);

	OutToken = LoginSession.GetLoginToken(VIVOX_VOICE_KEY, TokenExpiration);
}

void FVivoxToken::GenerateClientJoinToken(const IChannelSession& ChannelSession, FString& OutToken)
{
	FTimespan TokenExpiration = FTimespan::FromSeconds(90);

	OutToken = ChannelSession.GetConnectToken(VIVOX_VOICE_KEY, TokenExpiration);
}

void UVirtual_life_GameInstance::Init()
{
	Super::Init();

}

void UVirtual_life_GameInstance::LoginToVivox()
{
	VivoxCore = &FModuleManager::LoadModuleChecked<FVivoxCoreModule>("VivoxCore");
	VivoxClient = &static_cast<FVivoxCoreModule*>(&FModuleManager::Get().LoadModuleChecked(TEXT("VivoxCore")))->VoiceClient();
	VivoxClient->Initialize();

	LoggedInAccountID = AccountId(VIVOX_VOICE_ISSUER, StrID, VIVOX_VOICE_DOMAIN);
	ILoginSession& LoginSession = VivoxClient->GetLoginSession(LoggedInAccountID);

	FString LoginToken;
	FVivoxToken::GenerateClientLoginToken(LoginSession, LoginToken);

	ILoginSession::FOnBeginLoginCompletedDelegate OnBeginLoginCompleteCallback;
	OnBeginLoginCompleteCallback.BindLambda([this, &LoginSession](VivoxCoreError Status)
		{
			if (Status == VxErrorSuccess)
			{
				UE_LOG(LogTemp, Log, TEXT("Vivox login successful"));
				bLoggedIn = true;
			}
			else if (Status == VxErrorInvalidState) // 1019
			{
				UE_LOG(LogTemp, Warning, TEXT("Vivox login failed - already logged in. Attempting logout..."));
				VivoxLogout();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Login failed: %s"), ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(Status)));
				BindLoginSessionHandlers(false, LoginSession);
			}
		});

	BindLoginSessionHandlers(true, LoginSession);
	VivoxCoreError ret = LoginSession.BeginLogin(VIVOX_VOICE_SERVER, LoginToken, OnBeginLoginCompleteCallback);
	UE_LOG(LogTemp, Log, TEXT("BeginLogin() called: %s"), ANSI_TO_TCHAR(FVivoxCoreModule::ErrorToString(ret)));
}

void UVirtual_life_GameInstance::BindLoginSessionHandlers(bool DoBind, ILoginSession& LoginSession)
{
	if (DoBind)
	{
		LoginSession.EventStateChanged.AddUObject(this, &UVirtual_life_GameInstance::OnLoginSessionStateChanged);
	}
	else
	{
		LoginSession.EventStateChanged.RemoveAll(this);
	}
}

void UVirtual_life_GameInstance::OnLoginSessionStateChanged(LoginState State)
{
	switch (State)
	{
	case LoginState::LoggedIn:
		// 기본 마이크 디바이스 설정
		VivoxClient->AudioOutputDevices().SetActiveDevice(VivoxClient->AudioOutputDevices().ActiveDevice());
		VivoxClient->AudioInputDevices().SetActiveDevice(VivoxClient->AudioInputDevices().ActiveDevice());
		VivoxClient->AudioInputDevices().SetMuted(false);
		VivoxClient->GetLoginSession(LoggedInAccountID).SetParticipantSpeakingUpdateRate(ParticipantSpeakingUpdateRate::Update1Hz);

		UE_LOG(LogTemp, Log, TEXT("Login success. Mic unmuted."));
		break;
	case LoginState::LoggingIn:
		UE_LOG(LogTemp, Log, TEXT("User is logging in..."));
		break;
	case LoginState::LoggedOut:
		UE_LOG(LogTemp, Log, TEXT("User has logged out."));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown login state."));
		break;
	}
}

void UVirtual_life_GameInstance::JoinChannel(FString ChannelName)
{
	if (!bLoggedIn || !VivoxClient)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot join channel: not logged in or VivoxClient is null."));
		return;
	}

	ILoginSession& LoginSession = VivoxClient->GetLoginSession(LoggedInAccountID);
	// 무조건 비공간 음성으로 세팅
	LoggedInChannelId = ChannelId(VIVOX_VOICE_ISSUER, ChannelName, VIVOX_VOICE_DOMAIN, ChannelType::NonPositional, Channel3DProperties(8100, 270, 1.0, EAudioFadeModel::InverseByDistance));
	IChannelSession& ChannelSession = LoginSession.GetChannelSession(LoggedInChannelId);

	FString JoinToken;
	FVivoxToken::GenerateClientJoinToken(ChannelSession, JoinToken);

	IChannelSession::FOnBeginConnectCompletedDelegate OnBeginConnectCompleteCallback;
	OnBeginConnectCompleteCallback.BindLambda([this, &LoginSession, &ChannelSession](VivoxCoreError Status)
		{
			if (VxErrorSuccess != Status)
			{
				BindChannelSessionHandlers(false, ChannelSession); // Unbind handlers if we fail to join
				LoginSession.DeleteChannelSession(ChannelSession.Channel()); // Disassociate this ChannelSession from the LoginSession
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Successfully joined channel %s"), *ChannelSession.Channel().Name());
				LoginSession.SetTransmissionMode(TransmissionMode::Single, { ChannelSession.Channel() });
			}
		});
	BindChannelSessionHandlers(true, ChannelSession);
	ChannelSession.BeginConnect(true, false, true, JoinToken, OnBeginConnectCompleteCallback);
}

void UVirtual_life_GameInstance::BindChannelSessionHandlers(bool DoBind, IChannelSession& ChannelSession)
{
	if (DoBind)
	{
		ChannelSession.EventAfterParticipantAdded.AddUObject(this, &UVirtual_life_GameInstance::OnChannelParticipantAdded);
		ChannelSession.EventBeforeParticipantRemoved.AddUObject(this, &UVirtual_life_GameInstance::OnChannelParticipantRemoved);
		ChannelSession.EventAfterParticipantUpdated.AddUObject(this, &UVirtual_life_GameInstance::OnChannelParticipantUpdated);
		ChannelSession.EventAudioStateChanged.AddUObject(this, &UVirtual_life_GameInstance::OnChannelAudioStateChanged);
	}
	else
	{
		ChannelSession.EventAfterParticipantAdded.RemoveAll(this);
		ChannelSession.EventBeforeParticipantRemoved.RemoveAll(this);
		ChannelSession.EventAfterParticipantUpdated.RemoveAll(this);
		ChannelSession.EventAudioStateChanged.RemoveAll(this);
	}
}

// 누군가 들어옴
void UVirtual_life_GameInstance::OnChannelParticipantAdded(const IParticipant& Participant)
{
	ChannelId Channel = Participant.ParentChannelSession().Channel();
	UE_LOG(LogTemp, Log, TEXT("User %s has joined channel %s (self = %s)"), *Participant.Account().Name(), *Channel.Name(), Participant.IsSelf() ? TEXT("true") : TEXT("false"));
	std::string DisplayName = std::string(TCHAR_TO_UTF8(*Participant.Account().Name()));
	if (DisplayName.rfind('-') != std::string::npos) {
		DisplayName = DisplayName.erase(DisplayName.rfind('-'));
	}
}

// 누군가 나감
void UVirtual_life_GameInstance::OnChannelParticipantRemoved(const IParticipant& Participant)
{
	ChannelId Channel = Participant.ParentChannelSession().Channel();
	UE_LOG(LogTemp, Log, TEXT("User %s has left channel %s (self = %s)"), *Participant.Account().Name(), *Channel.Name(), Participant.IsSelf() ? TEXT("true") : TEXT("false"));
}

// 참가자 상태 변화(말하는지, 음소거인지)
void UVirtual_life_GameInstance::OnChannelParticipantUpdated(const IParticipant& Participant)
{
	const FString Name = Participant.Account().Name();
	float Energy = Participant.AudioEnergy(); // 0.0 ~ 1.0
	UE_LOG(LogTemp, Log, TEXT("[%s] AudioEnergy: %.3f"), *Participant.Account().Name(), Energy);
}

// 연결 성공 시 송신 설정
void UVirtual_life_GameInstance::OnChannelAudioStateChanged(const IChannelConnectionState& State)
{

	if (State.State() == ConnectionState::Connected)
	{
		ILoginSession& LoginSession = VivoxClient->GetLoginSession(LoggedInAccountID);
		LoginSession.SetTransmissionMode(TransmissionMode::Single, { State.ChannelSession().Channel() });

		UE_LOG(LogTemp, Log, TEXT("SetTransmissionMode(Single) applied to %s"), *State.ChannelSession().Channel().Name());
	}
}

void UVirtual_life_GameInstance::VivoxLogout()
{
	if (bLoggedIn && VivoxClient)
	{
		ILoginSession& LoginSession = VivoxClient->GetLoginSession(LoggedInAccountID);
		if (LoginSession.State() == LoginState::LoggedIn)
		{
			LoginSession.Logout();
		}
	}

	Super::Shutdown();
}

