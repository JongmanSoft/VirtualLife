// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeOfDayManager.h"
#include "TimeOfDayInterface.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "VirtualLifeController.h"
#include "GamePlayMainWidget.h"
#include "ClockWidget.h"


ATimeOfDayManager::ATimeOfDayManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATimeOfDayManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimeOfDayManager::IncrementTime()
{
    ServerTime = FMath::Fmod(ServerTime + 0.01f, 24.0f);

    if (bIsTimerRunning)
        SeverUpdateSun();
    else
        ClientUpdateSun();

    // 撤/广 傈券 眉农
    if (ServerTime >= 6.0f && ServerTime < 18.0f)
    {
        if (!bIsDaytime)
        {
            bIsDaytime = true;
        }
    }
    else
    {
        if (bIsDaytime)
        {
            bIsDaytime = false;
        }
    }

    int32 SeverHours = FMath::FloorToInt(ServerTime);
    int32 ClientHours = FMath::FloorToInt(ClientTime);

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    AVirtualLifeController* VLController = Cast<AVirtualLifeController>(PC);
    if (!VLController) return;

    UGamePlayMainWidget* MainUI = Cast<UGamePlayMainWidget>(VLController->CurrentUI);
    if (!MainUI) return;

    UClockWidget* ClockWidget = MainUI->GetClockWidget();
    if (ClockWidget)
    {
        if (bIsTimerRunning) {
            ClockWidget->SetTimeText(SeverHours, 0.0f);
        }
        else {
            ClockWidget->SetTimeText(ClientHours, 0.0f);
        }
    }
}

void ATimeOfDayManager::SeverUpdateSun()
{
    if (!SunLight || !MoonLight) {
        return;
    }

    float BaseAngle = (ServerTime > 18.0f) ? 90.0f : -270.0f;
    float SunPitch = (ServerTime / 24.0f) * 360.0f + BaseAngle;
    SunLight->SetActorRotation(FRotator(SunPitch, 0.0f, 0.0f));

    MoonLight->GetLightComponent()->SetVisibility(!bIsDaytime);
    SunLight->GetLightComponent()->SetVisibility(bIsDaytime);
}

void ATimeOfDayManager::ClientUpdateSun()
{
    if (!SunLight || !MoonLight) {
        return;
    }

    float BaseAngle = (ClientTime > 18.0f) ? 90.0f : -270.0f;
    float SunPitch = (ClientTime / 24.0f) * 360.0f + BaseAngle;
    SunLight->SetActorRotation(FRotator(SunPitch, 0.0f, 0.0f));

    MoonLight->GetLightComponent()->SetVisibility(true);
    SunLight->GetLightComponent()->SetVisibility(false);
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

void ATimeOfDayManager::StartTimer()
{
    LocalReceiveTime = GetWorld()->GetRealTimeSeconds();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATimeOfDayManager::IncrementTime, 0.1, true);
}