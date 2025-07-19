// Fill out your copyright notic in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sockets.h"           
#include "Networking.h"      
#include "Virtual_life_project/Virtual_life_project.h"
#include "../Network/NetworkManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h" 
#include "Sound/SoundCue.h"       
#include "Virtual_life_project/Virtual_life_projectCharacter.h"
#include "../Player_Data/Player_data.h"
#include "../Player_Data/PlayerInventory.h"
#include "../Custom/Custom_data.h"
#include "../Player_Data/Quest_Manager.h"
#include "../marry_system/marry_manager.h"
#include "../Building/ObjectData.h"
#include "VivoxCoreCommon.h"
#include "VivoxCore.h"
#include <mutex>
#include "Virtual_life_GameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatReceived, const FString&, ChatMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatWithID, const int32&, from_id, const FString&, chat_msg);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateDoor, const uint8&, door_id, bool, is_open);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const uint8&, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, const int32&, gold_offset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldUpdated, const int32&, Final_gold);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestUpdated);

struct SpawnInfo
{
	PlayerInfo pinfo;
	Customizing cinfo;
	ACharacter* character;
};

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UVirtual_life_GameInstance : public UGameInstance, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UVirtual_life_GameInstance();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> PlayerClass;
public:

	//이벤트
	UPROPERTY(BlueprintAssignable)
	FOnChatReceived OnChatReceived; // 블루프린트에서 이벤트 바인딩 가능!

	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGoldChanged OnGoldChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGoldUpdated OnGoldUpdated;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentGold() const;

	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQusetUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnChatWithID OnChatWithID;

	UPROPERTY(BlueprintAssignable)
	FUpdateDoor UpdateDoor;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UFloatingTextWidget> FloatingTextWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") // 파티초대 ui
	TSubclassOf<class UNoticeFriendUIWidget> NoticeWidgetClass;

	UFUNCTION(BlueprintCallable)
	void ShowFloatingText(const FString& Text, const FLinearColor& Color, const FVector& WorldLocation);

public:
	void OnStart();

	// 내 방 관련
	FName MyRoomMapName = TEXT("RoomPlayMap");
	TArray<Object> CachedRoomObjects; // 서버에서 받은 내 방 건물들

	UFUNCTION(BlueprintCallable)
	void SendEnterRoom(FString roomID); // 서버에 방 진입 요청

	UFUNCTION(BlueprintCallable)
	void SendEnterMyRoom(); // 서버에 방 진입 요청

	void HandleRoomSetup(const SC_ROOM_SETUP_PACKET& p);

	UFUNCTION(BlueprintCallable)
	void SpawnCachedRoomObjects();

	UFUNCTION(BlueprintCallable)
	void SpawnRoomObjectsFromData(const TArray<FObjectData>& ObjectList);

	// network
	UFUNCTION(BlueprintCallable)
	void ConnectServer(FString addr);

	UFUNCTION(BlueprintCallable)
	void DisconnectServer();

	UFUNCTION(BlueprintCallable)
	void SendLoginInfoPacket(FString s, FString pw);

	UFUNCTION(BlueprintCallable)
	void SpawnPlayer();

	UFUNCTION(BlueprintCallable)
	void SpawnPlayerInRoom();

	UFUNCTION(BlueprintCallable)
	void SendChatPacket(FString s);

	UFUNCTION(BlueprintCallable)
	void SendLeavePacket();

	UFUNCTION(BlueprintCallable)
	FString GetName() { return name; }

	UFUNCTION(BlueprintCallable)
	void SendGetItemPacket(uint8 item_id, int num);

	UFUNCTION(BlueprintCallable)
	void SendEnterGamePacket();

	UFUNCTION(BlueprintCallable)
	void SendUpadteCustomPacket();

	UFUNCTION(BlueprintCallable)
	void SendUpdateGoldPacket(int cs_gold_offset);

	UFUNCTION(BlueprintCallable)
	void SendGetQuestPacket(uint8 giver_id, uint8 quest_id);

	UFUNCTION(BlueprintCallable)
	void SendRemoveQuestPacket(uint8 giver_id, uint8 quest_id);

	UFUNCTION(BlueprintCallable)
    void SendPlaceBuildPacket(const TArray<FObjectData>& Objects);

	UFUNCTION(BlueprintCallable)
	void SendRemoveBuildPacket(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	void SendUpdateBuildPacket(const FVector& OldLoc, const FVector& NewLoc, float NewYaw);

	UFUNCTION(BlueprintCallable)
	void SendRoomLeavePacket();

	UFUNCTION(BlueprintCallable)
	void SendPartyUpdatePacket(const FString& Id_str);

	UFUNCTION(BlueprintCallable)
	void SendPartyJoinPacket(const FString& Id_str);

	UFUNCTION(BlueprintCallable)
	void SendPartyRejectPacket(const FString& Id_str);

	//문열고 닫음 ㅠㅠ
	UFUNCTION(BlueprintCallable)
	void SendDoorStatePacket(const uint8& door_id, bool is_open);

	//새로운 자식을 만들었어요!
	void SendCreateKidPacket(unsigned int preg_id,
		unsigned int spouse_id, Customizing c, float x, float y, float z, float yaw,char personality);

	bool SendEnqueue(void* packet, int32 PacketSize);

	void ResetPlayers();

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UVirtual_life_GameInstance, STATGROUP_Tickables); }
	
	UFUNCTION(BlueprintCallable)
	virtual void Shutdown() override;  // 게임 종료 시 실행될 함수

	UFUNCTION(BlueprintCallable)
	void SendPlayerLocationToServer();

	// 기본 세팅
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1"); // ip주소
	int16 Port = PORT_NUM; // 포트 번호: 임의로 지정.

	// 패킷 관련
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<TArray<uint8>> SendPacketQueue;

	std::mutex lock;

	// 여기 이제 수정해야 함
	TMap<int, SpawnInfo> OtherPlayers;
	TArray<FString> chats;

public:
	// 건축 관련
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	TSubclassOf<class APlaceBuildActor> PlaceBuildClass;

	UPROPERTY()
	UDataTable* BuildingDataTable = nullptr;

public:
	//장비,아이템을 관리하는 인벤토리클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UPlayerInventory* m_inventory;

public: 
	//커스텀데이터를 저장하는 커스텀클래스 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	UCustom_data* m_custom;
	void custom_data_update(UCustom_data* targer_data, Customizing recv_cus);
	void custom_packet_setup(Customizing& targer_data, const UCustom_data* recv_cus);

public:
	//퀘스트들을 관리하는 퀘스트매니저클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	UQuest_Manager* m_quest;
public:
	//결혼정보를 관리하는 결혼매니저클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marry")
	Umarry_manager* m_marry;

	UFUNCTION()
	ACharacter* draw_one_player(int draw_id);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "time")
	float enter_time =0.0f;
public:
	
	UPROPERTY()
	UAudioComponent* BGMaudio;

	// BGM 재생 함수
	UFUNCTION(BlueprintCallable)
	void PlayBGM(USoundCue* BGMSoundCue);

	// BGM 정지 함수
	UFUNCTION(BlueprintCallable)
	void StopBGM();
	
	
	// 맵전환때문에
public:
	std::atomic_bool loaded = false;
	UFUNCTION(BlueprintCallable)
	void SetLoaded(bool b) 
	{ 
		loaded = b; 
	}
	UFUNCTION(BlueprintCallable)
	void SetNullAllPlayer()
	{
		for (auto& Pair : OtherPlayers)
		{
			Pair.Value.character->Destroy();
			Pair.Value.character = nullptr;
		}
	}

public:
	UFUNCTION(BlueprintCallable)
	int get_my_player_id();
private:
	class RecvManager* RecvThread = nullptr;
	class SendManager* SendThread = nullptr;
	int id;
	FString name = TEXT("김겜공");

	PlayerInfo MyPlayerInfo;  // 서버로부터 받은 위치 정보를 저장

	// 패킷 처리 함수
	void ProcessRecvPackets();
	float TimeAccumulator = 0.0f;  // 위치 전송 간격 관리
public :
	void set_name(FString _name) { name = _name; };
	void set_state(int state) { MyPlayerInfo.st = static_cast<STATE>(state); }
	virtual void Init() override;

// 메인 맵 -> 집 들어갈 때 위치 저장용
public:
	FVector LastMainMapLocation;
	FRotator LastMainMapRotation;

	bool bReturnFromRoom = false;

// 음성 채팅
public: 
	FString StrID;

	UFUNCTION(BlueprintCallable, Category = "Voice")
	void StartVoiceChat()
	{
		LoginToVivox();
	}

	void LoginToVivox();
	void BindLoginSessionHandlers(bool DoBind, ILoginSession& LoginSession);
	void OnLoginSessionStateChanged(LoginState State);
	void JoinChannel(FString ChannelName);
	void BindChannelSessionHandlers(bool DoBind, IChannelSession& ChannelSession);
	void VivoxLogout();
	void OnChannelParticipantAdded(const IParticipant& Participant);
	void OnChannelParticipantRemoved(const IParticipant& Participant);
	void OnChannelParticipantUpdated(const IParticipant& Participant);
	void OnChannelAudioStateChanged(const IChannelConnectionState& State);

	FVivoxCoreModule* VivoxCore;

	IClient* VivoxClient = nullptr;
	AccountId LoggedInAccountID;
	ChannelId LoggedInChannelId;

	bool bLoggedIn = false;

// 문 동기화 관련
public:
	unsigned short current_door_id[13]; 
	bool current_is_open[13]; 
};
