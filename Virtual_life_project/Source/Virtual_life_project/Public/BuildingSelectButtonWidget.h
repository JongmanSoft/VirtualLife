// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "FBuildInfo.h"
#include "BuildingSelectButtonWidget.generated.h"

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UBuildingSelectButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativePreConstruct() override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void CheckActive();
    virtual void CheckActive_Implementation();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    UDataTable* DataTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    FName RowName;

    UPROPERTY(meta = (BindWidget))
    class UImage* AssetImage;

    UPROPERTY(meta = (BindWidget))
    class UButton* Build_BTN;

    UFUNCTION()
    void OnClickedBuildButton();
};
