// Fill out your copyright notice in the Description page of Project Settings.


#include "DateWidget.h"
#include "Components/TextBlock.h"


void UDateWidget::SetDateText(int32 Year, int32 Month, int32 Day)
{
    FString DateString = FString::Printf(TEXT("%d³â %02d¿ù %02dÀÏ"), Year, Month, Day);

    if (Date_TXT)
    {
        Date_TXT->SetText(FText::FromString(DateString));
    }
}

void UDateWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SetDateText(1801, 4, 22);
}
