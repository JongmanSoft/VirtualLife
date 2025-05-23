// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlacementActor.generated.h"

class APlaceBuildActor;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API APlacementActor : public AActor
{
	GENERATED_BODY()
	
public:
    APlacementActor();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    FName RowID;

    UFUNCTION()
    void SetRowID(FName InRowID);

    UFUNCTION()
    FName GetRowID() const { return RowID; }

    UFUNCTION()
    void SetMesh(UStaticMesh* NewMesh);

    UFUNCTION(BlueprintCallable)
    void PlaceBuild();

    UFUNCTION(BlueprintCallable)
    void AddRotation(float Delta);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<APlaceBuildActor> PlaceBuildClass;

    UPROPERTY(EditAnywhere, Category = "Material")
    UMaterialInterface* OverlayMaterial;

    UPROPERTY()
    UMaterialInstanceDynamic* DynMaterial = nullptr;

    bool IsOverlapping() const;
    bool IsOutOfBounds(const FVector& Location) const;
    void UpdateOverlayColor(bool bCanPlace);

    UPROPERTY()
    int32 BuildPrice = 0;

    void SetPrice(int32 Price);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* PlaceSound;

    UFUNCTION(BlueprintCallable, Category = "Placement")
    void AdjustScaleByWheel(float AxisValue);

protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    float GridSize = 10.f;

    UPROPERTY(EditAnywhere)
    float Rotate = 0.f;

    FVector MousePosition();

protected:
    float CurrentScale = 1.0f;

    const float MinScale = 0.2f;
    const float MaxScale = 3.0f;
    const float ScaleStep = 0.1f;
};
