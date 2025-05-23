// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BuildingCameraPawn.generated.h"

UCLASS()
class VIRTUAL_LIFE_PROJECT_API ABuildingCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABuildingCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
