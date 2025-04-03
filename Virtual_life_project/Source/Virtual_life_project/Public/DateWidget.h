// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DateWidget.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UDateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetDateText(int32 Year, int32 Month, int32 Day);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Year_TXT;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Month_TXT;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* Day_TXT;
};
