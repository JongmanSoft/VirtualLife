// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPlayGameMode.h"
#include "Virtual_life_GameInstance.h"


ARoomPlayGameMode::ARoomPlayGameMode()
{
}

void ARoomPlayGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (auto GI = Cast<UVirtual_life_GameInstance>(GetGameInstance()))
    {
        GI->SpawnCachedRoomObjects();
        UE_LOG(LogTemp, Error, TEXT("SpawnCachedRoomObjects Called!!!!!"));

    }
}