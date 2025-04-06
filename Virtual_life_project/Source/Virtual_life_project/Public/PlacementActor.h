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

    /** 위젯에서 설정되는 메쉬 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* BuildingMesh;

protected:
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    float GridSize = 100.f;

    FVector GetMouseSnappedPosition() const;
};
