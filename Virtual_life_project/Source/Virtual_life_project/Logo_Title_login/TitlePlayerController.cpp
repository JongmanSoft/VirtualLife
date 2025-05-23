// TitlePlayerController.cpp

#include "TitlePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Title_Widget.h"
#include "Name_Widget.h"

void ATitlePlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (TitleWidgetClass)
    {
        CurrentWidget = CreateWidget<UTitle_Widget>(this, TitleWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();

            SetInputMode(FInputModeUIOnly());
            bShowMouseCursor = true;

            UE_LOG(LogTemp, Warning, TEXT("Title UI Success"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Title UI Fail"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("WidgetClass Fail"));
    }
}

void ATitlePlayerController::SwitchToNameWidget()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("CurrentWidget Remove From Viewport Success"));
    }

    if (NameWidgetClass)
    {
        CurrentWidget = CreateWidget<UName_Widget>(this, NameWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("SwitchToNameWidget Success"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Name UI Fail"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NameWidgetClass Fail"));
    }
}

void ATitlePlayerController::SwitchToTitleWidget()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("CurrentWidget Remove"));
    }

    if (TitleWidgetClass)
    {
        CurrentWidget = CreateWidget<UTitle_Widget>(this, TitleWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("SwitchToTitleWidget Success"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("SwitchToTitleWidget Fail"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("TitleWidgetClass Missing"));
    }
}
