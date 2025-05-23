// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "mining_spot.generated.h"

UCLASS()
class VIRTUAL_LIFE_PROJECT_API Amining_spot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Amining_spot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
