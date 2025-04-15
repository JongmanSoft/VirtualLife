// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sockets.h"           
#include "Networking.h"      
#include "Virtual_life_project/Virtual_life_project.h"
#include "../Network/NetworkManager.h"
#include "Kismet/GameplayStatics.h"
#include "Virtual_life_project/Virtual_life_projectCharacter.h"
#include "Player_data.h"
#include "PlayerInventory.h"
#include "Custom_data.h"
#include "Quest_Manager.h"
#include "ObjectData.h"
#include <mutex>
#include "Virtual_life_GameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatReceived, const FString&, ChatMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, const uint8&, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, const int32&, gold_offset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldUpdated, const int32&, Final_gold);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestUpdated);

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

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UFloatingTextWidget> FloatingTextWidgetClass;

	UFUNCTION(BlueprintCallable)
	void ShowFloatingText(const FString& Text, const FLinearColor& Color, const FVector& WorldLocation);

public:
	void OnStart();

	// 내 방 관련
	FName MyRoomMapName = TEXT("RoomPlayMap");
	TArray<Object> CachedRoomObjects; // 서버에서 받은 내 방 건물들

	UFUNCTION(BlueprintCallable)
	void EnterMyRoom(); // 서버에 방 진입 요청

	void HandleRoomSetup(const SC_ROOM_SETUP_PACKET& p);

	UFUNCTION(BlueprintCallable)
	void SpawnCachedRoomObjects();


	// network
	UFUNCTION(BlueprintCallable)
	void ConnectServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectServer();

	UFUNCTION(BlueprintCallable)
	void SendLoginInfoPacket(FString s);

	UFUNCTION(BlueprintCallable)
	void SpawnPlayer();

	UFUNCTION(BlueprintCallable)
	void SendChatPacket(FString s);

	UFUNCTION(BlueprintCallable)
	void SendLeavePacket();

	UFUNCTION(BlueprintCallable)
	FString GetName() { return name; }

	UFUNCTION(BlueprintCallable)
	void SendGetItemPacket(uint8 item_id, uint8 num);

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

	bool SendEnqueue(void* packet, int32 PacketSize);

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UVirtual_life_GameInstance, STATGROUP_Tickables); }
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
	UPROPERTY()
	TMap<int, ACharacter*> SpawnedPlayers;

	TArray<std::pair<PlayerInfo, Customizing>> NeedSpawnPoints;

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

private:
	std::atomic_bool loaded = false;
	class RecvManager* RecvThread = nullptr;
	class SendManager* SendThread = nullptr;
	int id;
	FString name = TEXT("김겜공");
	// todo: 이거 옮길지 고민
	PlayerInfo MyPlayerInfo;  // 서버로부터 받은 위치 정보를 저장
	AdditionalInfo AddInfo;

	// 패킷 처리 함수
	void ProcessRecvPackets();
	float TimeAccumulator = 0.0f;  // 위치 전송 간격 관리
public :
	void set_name(FString _name) { name = _name; };
	void set_state(int state) { MyPlayerInfo.st = static_cast<STATE>(state); }
};
