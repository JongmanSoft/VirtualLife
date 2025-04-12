// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingInteractionWidget.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UBuildingInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnClickModify();

    UFUNCTION()
    void OnClickDelete();

public:
    UPROPERTY(meta = (BindWidget))
    UButton* Modify_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* Delete_BTN;
};
