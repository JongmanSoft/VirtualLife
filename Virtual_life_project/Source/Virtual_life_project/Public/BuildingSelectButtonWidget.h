// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "FBuildInfo.h"
#include "BuildingSelectButtonWidget.generated.h"

/**
 * 
 */
class UButton;
class UImage;
class APlacementActor;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UBuildingSelectButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UImage* AssetImage;

    UPROPERTY(meta = (BindWidget))
    UButton* Build_BTN;

    // DataTable 및 선택 Row
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data")
    UDataTable* DataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data")
    FDataTableRowHandle DataRow;

    // 스폰할 액터 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data")
    TSubclassOf<APlacementActor> PlacementActorClass;

protected:
    UFUNCTION()
    void OnBuildButtonClicked();

    void CheckActive();
};
