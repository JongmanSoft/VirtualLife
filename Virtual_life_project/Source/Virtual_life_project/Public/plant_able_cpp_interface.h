// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "plant_able_cpp_interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class Uplant_able_cpp_interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VIRTUAL_LIFE_PROJECT_API Iplant_able_cpp_interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Plant")
	void plant_what_func(int32 plant, bool& ok);
};
