// Fill out your copyright notice in the Description page of Project Settings.


#include "ClockWidget.h"
#include "Components/TextBlock.h"

void UClockWidget::SetTimeText(int32 Hours, int32 Minutes)
{
    FString HourStr = (Hours < 10) ? FString::Printf(TEXT("0%d"), Hours) : FString::FromInt(Hours);
    FString MinStr = (Minutes < 10) ? FString::Printf(TEXT("0%d"), Minutes) : FString::FromInt(Minutes);
    FString FinalTime = HourStr + ":" + MinStr;

    if (Clock_TXT)
    {
        Clock_TXT->SetText(FText::FromString(FinalTime));
    }
}

void UClockWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
