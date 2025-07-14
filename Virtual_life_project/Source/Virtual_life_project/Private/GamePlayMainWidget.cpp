// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayMainWidget.h"
#include "../Time/ClockWidget.h"
#include "../Time/WeatherWidget.h"
#include "../Time/DateWidget.h"
#include "MyProfileWidget.h"
#include "Components/Button.h"
#include "Virtual_life_GameInstance.h"

void UGamePlayMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (HomeMode_WB)
    {
        HomeMode_WB->OnClicked.AddDynamic(this, &UGamePlayMainWidget::OnHomeModeClicked);
    }

}

void UGamePlayMainWidget::OnHomeModeClicked()
{
    if (RoomJoinWidgetClass)
    {
        UUserWidget* RoomWidget = CreateWidget<UUserWidget>(GetWorld(), RoomJoinWidgetClass);
        if (RoomWidget)
        {
            RoomWidget->AddToViewport();
        }
    }
}
