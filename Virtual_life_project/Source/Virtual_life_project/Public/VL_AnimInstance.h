// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Virtual_life_GameInstance.h"
#include "VL_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API UVL_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	STATE state;

public:
	void setState(STATE s) { state = s; }
	STATE getState() { return state; }
};
