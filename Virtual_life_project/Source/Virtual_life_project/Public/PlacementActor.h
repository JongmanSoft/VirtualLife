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

protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    float GridSize = 100.f;

    UPROPERTY(EditAnywhere)
    float Rotate = 0.f;

    FVector MousePosition();
};
