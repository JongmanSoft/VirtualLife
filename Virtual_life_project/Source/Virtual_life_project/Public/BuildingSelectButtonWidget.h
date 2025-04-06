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

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UBuildingSelectButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Build")
    void OnBuildButtonClicked();
    virtual void OnBuildButtonClicked_Implementation();
};
