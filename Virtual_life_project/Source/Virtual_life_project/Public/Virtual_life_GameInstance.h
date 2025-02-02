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

	PlayerInfo MyPlayerInfo;  // 서버로부터 받은 위치 정보를 저장

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


	// 기본 세팅
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1"); // ip주소
	int16 Port = PORT_NUM; // 포트 번호: 임의로 지정.

	// 패킷 관련
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<TArray<uint8>> SendPacketQueue;

private:
	class RecvManager* RecvThread = nullptr;
	class SendManager* SendThread = nullptr;

	// 패킷 처리 함수
	void ProcessRecvPackets();
};
