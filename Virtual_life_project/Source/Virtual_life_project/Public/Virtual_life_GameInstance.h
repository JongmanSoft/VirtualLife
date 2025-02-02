// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sockets.h"           
#include "Networking.h"      
#include "Virtual_life_project/Virtual_life_project.h"
#include "../Network/NetworkManager.h"
#include "Kismet/GameplayStatics.h"
#include "Virtual_life_GameInstance.generated.h"

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UVirtual_life_GameInstance : public UGameInstance, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	void OnStart();
	void OnLevelLoaded(UWorld* LoadedWorld);

	PlayerInfo MyPlayerInfo;  // �����κ��� ���� ��ġ ������ ����

	// network
	UFUNCTION(BlueprintCallable)
	void ConnectServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectServer();

	UFUNCTION(BlueprintCallable)
	void SendLoginInfoPacket(FString s);

	bool SendEnqueue(void* packet, int32 PacketSize);

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return true; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UVirtual_life_GameInstance, STATGROUP_Tickables); }


	// �⺻ ����
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1"); // ip�ּ�
	int16 Port = PORT_NUM; // ��Ʈ ��ȣ: ���Ƿ� ����.

	// ��Ŷ ����
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<TArray<uint8>> SendPacketQueue;

private:
	class RecvManager* RecvThread = nullptr;
	class SendManager* SendThread = nullptr;

	// ��Ŷ ó�� �Լ�
	void ProcessRecvPackets();
};
