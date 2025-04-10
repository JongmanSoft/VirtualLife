// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API UFloatingTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void SetTextAndPlay(const FString& Text, const FLinearColor& Color);

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* FloatingText;

    /*UPROPERTY(Transient, meta = (BindWidgetAnim))
    class UWidgetAnimation* FloatAndFade;*/
};
