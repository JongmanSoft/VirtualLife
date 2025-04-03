// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeOfDayManager.h"
#include "TimeOfDayInterface.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ATimeOfDayManager::ATimeOfDayManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATimeOfDayManager::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATimeOfDayManager::IncrementTime, 0.1, true);
}

void ATimeOfDayManager::IncrementTime()
{
    Time = FMath::Fmod(Time + TimeStep, 24.0f);

    UpdateSun();

    // 撤/广 傈券 眉农
    if (Time >= 6.0f && Time < 18.0f)
    {
        if (!bIsDaytime)
        {
            bIsDaytime = true;
            OnSunrise();
            NotifyListeners(true);
        }
    }
    else
    {
        if (bIsDaytime)
        {
            bIsDaytime = false;
            OnSunset();
            NotifyListeners(false);
        }
    }
}

void ATimeOfDayManager::UpdateSun()
{
    if (!SunLight || !MoonLight) return;

    float SunPitch = FMath::Fmod((Time / 24.0f) * 360.0f + 270.0f, 360.0f);
    SunLight->SetActorRotation(FRotator(SunPitch, 0.0f, 0.0f));

    MoonLight->GetLightComponent()->SetVisibility(!bIsDaytime);
    SunLight->GetLightComponent()->SetVisibility(bIsDaytime);
}

void ATimeOfDayManager::NotifyListeners(bool bDaytime)
{
    // Actor 贸府
    TArray<AActor*> Actors;
    TSubclassOf<UInterface> InterfaceClass = UTimeOfDayInterface::StaticClass();

    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), InterfaceClass, Actors);

    for (AActor* Actor : Actors)
    {
        if (bDaytime)
        {
            ITimeOfDayInterface::Execute_NotifySunrise(Actor);
        }
        else
        {
            ITimeOfDayInterface::Execute_NotifySunset(Actor);
        }
    }

    // Widget 贸府
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsWithInterface(
        GetWorld(), Widgets, InterfaceClass, false);

    for (UUserWidget* Widget : Widgets)
    {
        if (bDaytime)
        {
            ITimeOfDayInterface::Execute_NotifySunrise(Widget);
        }
        else
        {
            ITimeOfDayInterface::Execute_NotifySunset(Widget);
        }
    }
}

void ATimeOfDayManager::OnSunrise()
{
}

void ATimeOfDayManager::OnSunset()
{
}

