// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGameMode.h"
#include "BuildingCameraPawn.h"
#include "BuildingPlayerController.h"
#include "Virtual_life_GameInstance.h"

ABuildingGameMode::ABuildingGameMode()
{
    DefaultPawnClass = ABuildingCameraPawn::StaticClass();
    PlayerControllerClass = ABuildingPlayerController::StaticClass();
}

void ABuildingGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (auto GI = Cast<UVirtual_life_GameInstance>(GetGameInstance()))
    {
        GI->SpawnCachedRoomObjects();
        UE_LOG(LogTemp, Error, TEXT("SpawnCachedRoomObjects Called!!!!!"));

    }
}