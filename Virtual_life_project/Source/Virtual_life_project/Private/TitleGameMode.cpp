// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "TitlePlayerController.h"


ATitleGameMode::ATitleGameMode()
{
	PlayerControllerClass = ATitlePlayerController::StaticClass();
}
