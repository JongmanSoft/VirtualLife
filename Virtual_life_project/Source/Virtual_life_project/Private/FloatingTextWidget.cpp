// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingTextWidget.h"
#include "Components/TextBlock.h"

void UFloatingTextWidget::SetTextAndPlay(const FString& Text, const FLinearColor& Color)
{
    if (FloatingText)
    {
        FloatingText->SetText(FText::FromString(Text));
        FloatingText->SetColorAndOpacity(FSlateColor(Color));
    }

    // 애니메이션 재생
    /*if (FloatAndFade)
    {
        PlayAnimation(FloatAndFade);
    }*/
}