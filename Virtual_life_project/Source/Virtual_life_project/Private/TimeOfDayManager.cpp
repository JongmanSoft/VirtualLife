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
	
	//SGetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATimeOfDayManager::IncrementTime, 0.1, true);
}

void ATimeOfDayManager::IncrementTime()
{
    Time = FMath::Fmod(Time + TimeStep, 24.0f);
    UE_LOG(LogTemp, Log, TEXT("HI"));

    UpdateSun();

    // 낮/밤 전환 체크
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

    // 시간 계산
    int32 Hours = FMath::FloorToInt(Time);
    int32 Minutes = FMath::FloorToInt(FMath::Fmod(Time * 60.0f, 60.0f));

    // 플레이어 컨트롤러 → VLController
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    AVirtualLifeController* VLController = Cast<AVirtualLifeController>(PC);
    if (!VLController) return;

    // CurrentUI → GamePlayMainWidget 캐스팅
    UGamePlayMainWidget* MainUI = Cast<UGamePlayMainWidget>(VLController->CurrentUI);
    if (!MainUI) return;

    // Clock_W 위젯 가져오기
    UClockWidget* ClockWidget = MainUI->GetClockWidget();
    if (ClockWidget)
    {
        ClockWidget->SetTimeText(Hours, Minutes);
    }
}

void ATimeOfDayManager::UpdateSun()
{
    if (!SunLight || !MoonLight) {
        UE_LOG(LogTemp, Log, TEXT("No Light"));
        return;
    }

    float BaseAngle = (Time > 18.0f) ? 90.0f : -270.0f;
    float SunPitch = (Time / 24.0f) * 360.0f + BaseAngle;
    SunLight->SetActorRotation(FRotator(SunPitch, 0.0f, 0.0f));

    MoonLight->GetLightComponent()->SetVisibility(!bIsDaytime);
    SunLight->GetLightComponent()->SetVisibility(bIsDaytime);
}

void ATimeOfDayManager::NotifyListeners(bool bDaytime)
{
    // Actor 처리
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

    // Widget 처리
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
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATimeOfDayManager::IncrementTime, 0.1, true);
}

