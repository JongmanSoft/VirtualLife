// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPI_interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPI_interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VIRTUAL_LIFE_PROJECT_API ICPI_interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "interact")
	void Interact();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "interact")
	void Can_Receive_Trace(bool Can_Interact);

};
