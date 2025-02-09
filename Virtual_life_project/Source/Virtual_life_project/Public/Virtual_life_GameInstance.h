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
#include "PlayerInventory.h"
#include <mutex>
#include "Virtual_life_GameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatReceived, const FString&, ChatMessage);

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UVirtual_life_GameInstance : public UGameInstance, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UVirtual_life_GameInstance();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AVirtual_life_projectCharacter> PlayerClass;

	UPROPERTY(BlueprintAssignable)
	FOnChatReceived OnChatReceived; // 블루프린트에서 이벤트 바인딩 가능!

	void OnStart();

	PlayerInfo MyPlayerInfo;  // 서버로부터 받은 위치 정보를 저장

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

	bool SendEnqueue(void* packet, int32 PacketSize);

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UVirtual_life_GameInstance, STATGROUP_Tickables); }

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
	TMap<int, AVirtual_life_projectCharacter*> SpawnedPlayers;

	TArray<PlayerInfo> NeedSpawnPoints;

	TArray<FString> chats;
public:
	//장비,아이템을 관리하는 인벤토리클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UPlayerInventory* m_inventory;

private:
	std::atomic_bool loaded = false;
	class RecvManager* RecvThread = nullptr;
	class SendManager* SendThread = nullptr;
	int id;
	FString name;

	// 패킷 처리 함수
	void ProcessRecvPackets();
	float TimeAccumulator = 0.0f;  // 위치 전송 간격 관리
};
