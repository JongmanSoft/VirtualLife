// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceBuildActor.generated.h"

UCLASS()
class VIRTUAL_LIFE_PROJECT_API APlaceBuildActor : public AActor
{
	GENERATED_BODY()
	
public:
    APlaceBuildActor();

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UFUNCTION()
    void SetMesh(UStaticMesh* StaticMesh);

};
