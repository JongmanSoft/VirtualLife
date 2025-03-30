// Fill out your copyright notice in the Description page of Project Settings.


#include "Name_Widget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "TitlePlayerController.h"
#include "Virtual_life_GameInstance.h"


void UName_Widget::NativeConstruct()
{
    Super::NativeConstruct();

    if (OK_BTN)
    {
        OK_BTN->OnClicked.AddDynamic(this, &UName_Widget::OnOKClicked);
    }

    if (Back_BTN)
    {
        Back_BTN->OnClicked.AddDynamic(this, &UName_Widget::OnBackClicked);
    }
}

void UName_Widget::OnOKClicked()
{
    FString LastName = LastNameBox ? LastNameBox->GetText().ToString() : "";
    FString FirstName = FirstNameBox ? FirstNameBox->GetText().ToString() : "";

    UE_LOG(LogTemp, Warning, TEXT("성: %s, 이름: %s"), *LastName, *FirstName);

    // 여기에서 서버에 이름을 전달하거나 
    // 다음 UI/씬으로 이동하는 코드 작성
    UVirtual_life_GameInstance* my_instance = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
    if (my_instance)
    {
        my_instance->set_name(FirstName+LastName);
        my_instance->SendLoginInfoPacket(FirstName + LastName);
    }

    // custom map으로 이동?
    FName LevelName = FName("custom_map"); // custom 맵 이름 확인
    UGameplayStatics::OpenLevel(this, LevelName);
}

void UName_Widget::OnBackClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("back button clicked"));

    if (ATitlePlayerController* PC = Cast<ATitlePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
    {
        PC->SwitchToTitleWidget();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController Casting Failed (Back)"));
    }
}