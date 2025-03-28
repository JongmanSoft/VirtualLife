// Fill out your copyright notice in the Description page of Project Settings.


#include "Name_Widget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"


void UName_Widget::NativeConstruct()
{
    Super::NativeConstruct();

    if (OK_BTN)
    {
        OK_BTN->OnClicked.AddDynamic(this, &UName_Widget::OnOKClicked);
    }
}

void UName_Widget::OnOKClicked()
{
    FString LastName = LastNameBox ? LastNameBox->GetText().ToString() : "";
    FString FirstName = FirstNameBox ? FirstNameBox->GetText().ToString() : "";

    UE_LOG(LogTemp, Warning, TEXT("성: %s, 이름: %s"), *LastName, *FirstName);

    // 여기에서 서버에 이름을 전달하거나 
    // 다음 UI/씬으로 이동하는 코드 작성
}
