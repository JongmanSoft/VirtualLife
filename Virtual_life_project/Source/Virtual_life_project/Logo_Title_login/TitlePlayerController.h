// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UUserWidget* CurrentWidget;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UTitle_Widget> TitleWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UName_Widget> NameWidgetClass;

public:
    void SwitchToNameWidget();
    void SwitchToTitleWidget();
};
