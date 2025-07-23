// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "display_user_name.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API Udisplay_user_name : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* user_name_text;

	UFUNCTION(BlueprintCallable)
	void set_name(const FText& new_name);
};
