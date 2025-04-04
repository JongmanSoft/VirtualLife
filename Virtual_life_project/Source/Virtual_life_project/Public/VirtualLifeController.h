// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VirtualLifeController.generated.h"

/**
 * 
 */


UCLASS()
class VIRTUAL_LIFE_PROJECT_API AVirtualLifeController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AVirtualLifeController();

    /** 현재 UI 위젯을 보관하는 포인터 */
    UPROPERTY(BlueprintReadWrite, Category = "UI")
    class UUserWidget* CurrentUI;
};
