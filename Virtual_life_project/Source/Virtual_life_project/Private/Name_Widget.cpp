// Fill out your copyright notice in the Description page of Project Settings.


#include "Name_Widget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
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

    if (LastNameBox)
    {
        LastNameBox->OnTextCommitted.AddDynamic(this, &UName_Widget::OnLastNameBoxFocused);
    }

    if (FirstNameBox)
    {
        FirstNameBox->OnTextCommitted.AddDynamic(this, &UName_Widget::OnFirstNameBoxFocused);
    }
}

void UName_Widget::OnOKClicked()
{
    FString LastName = LastNameBox ? LastNameBox->GetText().ToString() : "";
    FString FirstName = FirstNameBox ? FirstNameBox->GetText().ToString() : "";

    UE_LOG(LogTemp, Warning, TEXT("성: %s, 이름: %s"), *LastName, *FirstName);

    // 게임 인스턴스에 이름 전달
    UVirtual_life_GameInstance* my_instance = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
    if (my_instance)
    {
        my_instance->set_name(FirstName + LastName);
        //my_instance->SendLoginInfoPacket(FirstName + LastName);
    }

    // custom 맵으로 전환
    FName LevelName = FName("custom_map"); // 맵 이름은 Level Browser 기준으로 정확히 확인
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

void UName_Widget::OnLastNameBoxFocused(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (LastNameBox && !bLastNameEdited)
    {
        LastNameBox->SetText(FText::GetEmpty());
        bLastNameEdited = true; 
    }
}

void UName_Widget::OnFirstNameBoxFocused(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (FirstNameBox && !bFirstNameEdited)
    {
        FirstNameBox->SetText(FText::GetEmpty());
        bFirstNameEdited = true; 
    }
}

void UName_Widget::OnLastNameChanged(const FText& Text)
{
}

void UName_Widget::OnFirstNameChanged(const FText& Text)
{
}

void UName_Widget::UpdateFullNameText()
{
}
