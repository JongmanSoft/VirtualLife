// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest.h"
#include "Quest_Manager.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class VIRTUAL_LIFE_PROJECT_API UQuest_Manager : public UObject
{
	GENERATED_BODY()

public:
	TArray <UQuest*> Quests;


};
