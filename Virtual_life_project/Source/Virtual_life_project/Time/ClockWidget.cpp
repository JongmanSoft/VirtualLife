// Fill out your copyright notice in the Description page of Project Settings.


#include "ClockWidget.h"
#include "Virtual_life_GameInstance.h"
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

void UClockWidget::UpdateFromGameInstance()
{
    UVirtual_life_GameInstance* GI = Cast<UVirtual_life_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!GI) return;

    float time = GI->currentSyncedTime;
    int32 hours = static_cast<int32>(FMath::FloorToInt(time));
    int32 minutes = static_cast<int32>((time - hours) * 60.0f);

    SetTimeText(hours, minutes);

}

void UClockWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
