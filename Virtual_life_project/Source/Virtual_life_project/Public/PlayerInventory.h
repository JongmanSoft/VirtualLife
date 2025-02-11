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


	UPlayerInventory(); // ������, �̸� �ʱ�ȭ�� TMap������ �־��ָ�����,,


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap <uint8,uint8> Owned_Items; //������ ID, ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray <uint8> Ownerd_Equip; //index : Ű(1~5) value: ��� ��� ��ȣ 
	
	UFUNCTION(BlueprintCallable)
	void Add_Item(uint8 Add_ID, uint8 Add_num); //������ ���̵�� ������ �޾� ���������ۿ� �߰��ϴ� �Լ�

	UFUNCTION(BlueprintCallable)
	void road_Item(TMap<uint8, uint8>New_Items);  //�������� �ϰ��� �ε��ϴ� �Լ�
	
	UFUNCTION(BlueprintCallable)
	void get_item(uint8 id, uint8 num); // ���� �۽�
	
	UFUNCTION(BlueprintCallable)
	void update_item(uint8 id, uint8 num); // ���� ����
};
