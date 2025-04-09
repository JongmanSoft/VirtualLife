// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPlayerController.h"

void ABuildingPlayerController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}