// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quset_ui.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API UQuset_ui : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestRequiredValue;
	
};
