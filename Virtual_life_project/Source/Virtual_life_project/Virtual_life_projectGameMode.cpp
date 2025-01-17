// Copyright Epic Games, Inc. All Rights Reserved.

#include "Virtual_life_projectGameMode.h"
#include "Virtual_life_projectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVirtual_life_projectGameMode::AVirtual_life_projectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
