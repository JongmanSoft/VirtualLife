// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayMainWidget.h"
#include "ClockWidget.h"
#include "WeatherWidget.h"
#include "DateWidget.h"
#include "MyProfileWidget.h"

void UGamePlayMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 확인용 로그
    UE_LOG(LogTemp, Log, TEXT("GamePlay_UI initialized"));
}