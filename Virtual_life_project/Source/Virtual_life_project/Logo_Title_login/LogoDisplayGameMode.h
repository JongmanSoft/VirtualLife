// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LogoDisplayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API ALogoDisplayGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MoveToLoginMap();
};
