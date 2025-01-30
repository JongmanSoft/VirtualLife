// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Sockets.h"           
#include "Networking.h"      
#include "Virtual_life_project/Virtual_life_project.h"
#include "Virtual_life_GameInstance.generated.h"

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UVirtual_life_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	// network
	UFUNCTION(BlueprintCallable)
	void ConnectServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectServer();

	// �⺻ ����
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1"); // ip�ּ�
	int16 Port = PORT_NUM; // ��Ʈ ��ȣ: ���Ƿ� ����.
};
