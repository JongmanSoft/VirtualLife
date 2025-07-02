// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "kid_map_script.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API Akid_map_script : public ALevelScriptActor
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;
};
