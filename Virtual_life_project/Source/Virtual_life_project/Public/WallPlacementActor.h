// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacementActor.h"
#include "WallPlacementActor.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API AWallPlacementActor : public APlacementActor
{
	GENERATED_BODY()
	
public:
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    void SetStartPoint(const FVector& InStart);
    void UpdateWallPreview(const FVector& Current);

    UFUNCTION(BlueprintCallable)

    void ConfirmWall(); // 최종 벽 생성

    void CancelWall(); // 임시 벽 삭제

private:
    FVector StartPoint;
    bool bHasStart = false;

public:
    FVector CachedMousePosition();
};
