// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlaceBuildActor.h"
#include "ObjectData.h"
#include "BuildingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API ABuildingPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	APlaceBuildActor* SelectedBuildActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<class APlacementActor> PlacementActorClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<FObjectData> PendingBuildObjects;

	void AddPendingBuild(const FObjectData& Data);
	void RemovePendingBuildAtLocation(const FVector& Location, float Tolerance = 10.f);

	UFUNCTION(BlueprintCallable)
	void TrySelectBuildActor();

};
