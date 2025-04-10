// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProfileWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Virtual_life_GameInstance.h"

void UMyProfileWidget::NativeConstruct()
{
    Super::NativeConstruct();

    Cast<UVirtual_life_GameInstance>(GetGameInstance())->OnGoldUpdated.AddDynamic(this, &UMyProfileWidget::SetCoin);

    SetHP(100, 100);
    SetMP(100, 100);
}

void UMyProfileWidget::SetHP(int32 Current, int32 Max)
{
    if (HP_TXT)
        HP_TXT->SetText(FText::Format(FText::FromString("HP ({0})"), Current));

    if (HP_Bar && Max > 0)
        HP_Bar->SetPercent(static_cast<float>(Current) / Max);
}

void UMyProfileWidget::SetMP(int32 Current, int32 Max)
{
    if (MP_TXT)
        MP_TXT->SetText(FText::Format(FText::FromString("MP ({0})"), Current));

    if (MP_Bar && Max > 0)
        MP_Bar->SetPercent(static_cast<float>(Current) / Max);
}

void UMyProfileWidget::SetCoin(const int32& updated_coin)
{
    Coin_TXT->SetText(FText::Format(FText::FromString("{0} Coin"), updated_coin));
}
