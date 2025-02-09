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
	FOnChatReceived OnChatReceived; // �������Ʈ���� �̺�Ʈ ���ε� ����!

	void OnStart();

	PlayerInfo MyPlayerInfo;  // �����κ��� ���� ��ġ ������ ����

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

	// �⺻ ����
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1"); // ip�ּ�
	int16 Port = PORT_NUM; // ��Ʈ ��ȣ: ���Ƿ� ����.

	// ��Ŷ ����
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<TArray<uint8>> SendPacketQueue;

	std::mutex lock;
	UPROPERTY()
	TMap<int, AVirtual_life_projectCharacter*> SpawnedPlayers;

	TArray<PlayerInfo> NeedSpawnPoints;

	TArray<FString> chats;
public:
	//���,�������� �����ϴ� �κ��丮Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UPlayerInventory* m_inventory;

private:
	std::atomic_bool loaded = false;
	class RecvManager* RecvThread = nullptr;
	class SendManager* SendThread = nullptr;
	int id;
	FString name;

	// ��Ŷ ó�� �Լ�
	void ProcessRecvPackets();
	float TimeAccumulator = 0.0f;  // ��ġ ���� ���� ����
};
