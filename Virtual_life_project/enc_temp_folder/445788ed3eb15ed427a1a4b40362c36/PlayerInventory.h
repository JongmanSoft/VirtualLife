// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerInventory.generated.h"

class UVirtual_life_GameInstance;

/**
 * 
 */
UCLASS(Blueprintable)
class VIRTUAL_LIFE_PROJECT_API UPlayerInventory : public UObject
{
	GENERATED_BODY()


public:


	UPlayerInventory(); // 생성자, 미리 초기화할 TMap잇으면 넣어주면좋코,,


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap <uint8,uint8> Owned_Items; //아이템 ID, 갯수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray <uint8> Ownerd_Equip; //index : 키(1~5) value: 사용 장비 번호 
	
	UFUNCTION(BlueprintCallable)
	void Add_Item(uint8 Add_ID, uint8 Add_num); //아이템 아이디와 갯수를 받아 소지아이템에 추가하는 함수

	UFUNCTION(BlueprintCallable)
	void road_Item(TMap<uint8, uint8>New_Items);  //아이템을 일괄로 로드하는 함수
	
	UFUNCTION(BlueprintCallable)
	void get_item(uint8 id, uint8 num); // 서버 송신
	
	UFUNCTION(BlueprintCallable)
	void update_item(uint8 id, uint8 num); // 서버 수신
};
