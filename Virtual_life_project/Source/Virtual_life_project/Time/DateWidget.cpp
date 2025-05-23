// Fill out your copyright notice in the Description page of Project Settings.


#include "DateWidget.h"
#include "Components/TextBlock.h"


void UDateWidget::SetDateText(int32 Year, int32 Month, int32 Day)
{
    if (Year_TXT)
        Year_TXT->SetText(FText::AsNumber(Year));

    if (Month_TXT)
        Month_TXT->SetText(FText::AsNumber(Month));

    if (Day_TXT)
        Day_TXT->SetText(FText::AsNumber(Day));
}
