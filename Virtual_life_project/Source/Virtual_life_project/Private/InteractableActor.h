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
	UPROPERTY()
	FName RowID;

	UFUNCTION(BlueprintCallable)
	void SetRowID(FName InRowID) { RowID = InRowID; }

	UFUNCTION(BlueprintCallable)
	FName GetRowID() const { return RowID; }

	UFUNCTION(BlueprintCallable)
	void SendStateToServer(uint8 StateValue);
};
