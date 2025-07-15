// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
    AInteractableActor();

    UFUNCTION(BlueprintCallable)
    void SendStateToServer(uint8 StateValue);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName RowID;

    UFUNCTION(BlueprintCallable)
    void SetRowID(FName InRowID) { RowID = InRowID; }

    UFUNCTION(BlueprintCallable)
    void SetMesh(UStaticMesh* StaticMesh);
};
