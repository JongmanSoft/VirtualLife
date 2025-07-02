// Fill out your copyright notice in the Description page of Project Settings.


#include "FreindUIWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Virtual_life_GameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UFreindUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Invite_BTN)
    {
        Invite_BTN->OnClicked.AddDynamic(this, &UFreindUIWidget::OnInviteClicked);
    }

    if (Close_BTN)
    {
        Close_BTN->OnClicked.AddDynamic(this, &UFreindUIWidget::OnCloseClicked);
    }
}

void UFreindUIWidget::OnInviteClicked()
{
    if (Invite_ID)
    {
        FString EnteredID = Invite_ID->GetText().ToString();

        if (!EnteredID.IsEmpty())
        {
            UVirtual_life_GameInstance* my_instance = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
            if (my_instance)
            {
				my_instance->SendPartyUpdatePacket(EnteredID);
            }
        }
    }
}

void UFreindUIWidget::OnCloseClicked()
{
    OnCloseRequested();
}