// Fill out your copyright notice in the Description page of Project Settings.


#include "Title_Widget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UTitle_Widget::NativeConstruct()
{
    // 버튼 이벤트 바인딩
    if (NewStart_BTN)
    {
        NewStart_BTN->OnClicked.AddDynamic(this, &UTitle_Widget::OnNewStartClicked);
    }

    if (Start_BTN)
    {
        Start_BTN->OnClicked.AddDynamic(this, &UTitle_Widget::OnStartClicked);
    }

    if (Quit_BTN)
    {
        Quit_BTN->OnClicked.AddDynamic(this, &UTitle_Widget::OnQuitClicked);
    }

    if (Option_BTN)
    {
        Option_BTN->OnClicked.AddDynamic(this, &UTitle_Widget::OnOptionClicked);
    }
}

void UTitle_Widget::OnNewStartClicked()
{
}

void UTitle_Widget::OnStartClicked()
{
}

void UTitle_Widget::OnQuitClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("게임 종료 클릭됨!"));
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

void UTitle_Widget::OnOptionClicked()
{
}
