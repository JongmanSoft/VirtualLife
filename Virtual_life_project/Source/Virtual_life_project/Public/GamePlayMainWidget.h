// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayMainWidget.generated.h"

/**
 * 
 */

class UClockWidget;
class UWeatherWidget;
class UDateWidget;
class UMyProfileWidget;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UGamePlayMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    UClockWidget* GetClockWidget() const { return Clock_W; }

protected:
    UPROPERTY(meta = (BindWidget))
    UClockWidget* Clock_W;

    UPROPERTY(meta = (BindWidget))
    UWeatherWidget* Weather_WW;

    UPROPERTY(meta = (BindWidget))
    UDateWidget* Date_W;

    UPROPERTY(meta = (BindWidget))
    UMyProfileWidget* MyProfile_UI;

    UPROPERTY(meta = (BindWidget))
    class UButton* HomeMode_WB;

    UFUNCTION()
    void OnHomeModeClicked();

};
