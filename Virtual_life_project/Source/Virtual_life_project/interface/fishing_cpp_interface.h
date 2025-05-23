// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "fishing_cpp_interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class Ufishing_cpp_interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VIRTUAL_LIFE_PROJECT_API Ifishing_cpp_interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Fishing")
	void fishing_func(bool& result);
};
