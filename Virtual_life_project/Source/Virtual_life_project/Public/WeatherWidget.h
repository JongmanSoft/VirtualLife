// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeOfDayInterface.h"
#include "WeatherWidget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API UWeatherWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativePreConstruct() override;

    // TimeOfDayInterface
    virtual void NotifySunrise_Implementation();
    virtual void NotifySunset_Implementation();

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* Weather_IMG;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    UTexture2D* Sun_ICN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    UTexture2D* Moon_ICN;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    UTexture2D* Cloud_ICN;

private:
    void SetWeather(UTexture2D* Icon);
};
