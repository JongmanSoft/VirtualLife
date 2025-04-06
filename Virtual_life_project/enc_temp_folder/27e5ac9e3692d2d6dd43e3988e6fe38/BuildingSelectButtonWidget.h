// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "FBuildInfo.h"
#include "BuildingSelectButtonWidget.generated.h"

class UButton;
class UImage;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UBuildingSelectButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void OnBuildButtonClicked();

    UFUNCTION(BlueprintCallable)
    void CheckActive();

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* Build_BTN;

    UPROPERTY(meta = (BindWidget))
    class UImage* AssetImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Build", meta = (ExposeOnSpawn = true))
    UDataTable* DataTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Build", meta = (ExposeOnSpawn = true))
    FName RowName;
};
