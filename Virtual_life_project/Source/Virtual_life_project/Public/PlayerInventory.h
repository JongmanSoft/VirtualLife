// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerInventory.generated.h"

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
	TMap <uint8, uint8> Ownerd_Equip; //단축키번호(1~5), 장착 장비 ID
	
	//todo: 이함수를 서버로 옮기고 클라는 소지아이템을 일괄로 받아오는 함수로 만들어야하지않나...
	UFUNCTION(BlueprintCallable)
	void Add_Item(uint8 Add_ID, uint8 Add_num); //아이템 아이디와 갯수를 받아 소지아이템에 추가하는 함수

	UFUNCTION(BlueprintCallable)
	void road_Item(TMap<uint8, uint8>New_Items);  //아이템을 일괄로 로드하는 함수
	
};
