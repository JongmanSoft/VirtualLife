// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "seed_spot.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VIRTUAL_LIFE_PROJECT_API Aseed_spot : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    Aseed_spot();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

public:
 
};
