// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlacementActor.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class AInteractableActor : public APlacementActor
{
	GENERATED_BODY()

public:
	AInteractableActor();

	UPROPERTY()
	FName RowID;

	void SetRowID(FName InRowID) { RowID = InRowID; }

	FName GetRowID() const { return RowID; }

	UFUNCTION(BlueprintCallable)
	void SendStateToServer(uint8 StateValue);
};
