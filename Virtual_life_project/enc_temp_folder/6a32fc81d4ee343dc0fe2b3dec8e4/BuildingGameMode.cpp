// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingGameMode.h"
#include "BuildingCameraPawn.h"
#include "BuildingPlayerController.h"

ABuildingGameMode::ABuildingGameMode()
{
    DefaultPawnClass = ABuildingCameraPawn::StaticClass();
    PlayerControllerClass = ABuildingPlayerController::StaticClass();
}
