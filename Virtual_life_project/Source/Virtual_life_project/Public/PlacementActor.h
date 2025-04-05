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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    // 컴포넌트
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    // 배치용 속성들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
    UStaticMesh* BuildingMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
    float GridSize = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
    float RotationAngle = 0.0f;

    FVector GetMouseSnappedPosition() const;

    UFUNCTION(BlueprintCallable, Category = "Placement")
    void PlaceBuild();

private:
    FVector CachedLocation;
};
