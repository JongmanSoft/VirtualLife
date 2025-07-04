// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Virtual_life_GameInstance.h"

void AInteractableActor::SendStateToServer(uint8 StateValue)
{
	if (auto* GI = Cast<UVirtual_life_GameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GI->SendInteractStateChange(StateValue);
	}
}