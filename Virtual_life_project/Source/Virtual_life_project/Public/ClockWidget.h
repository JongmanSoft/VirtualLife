// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClockWidget.generated.h"

/**
 * 
 */

class UTextBlock;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UClockWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void SetTimeText(int32 Hours, int32 Minutes);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Clock_TXT;
};
