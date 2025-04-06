// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacementActor.generated.h"

UCLASS()
class VIRTUAL_LIFE_PROJECT_API APlacementActor : public AActor
{
	GENERATED_BODY()
	
public:
    APlacementActor();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    UFUNCTION()
    void SetMesh(UStaticMesh* NewMesh);

    UFUNCTION()
    void PlaceBuild();

protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    float GridSize = 100.f;

    UPROPERTY(EditAnywhere)
    float Rotate = 0.f;

    FVector MousePosition();
};
