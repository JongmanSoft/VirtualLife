// Fill out your copyright notice in the Description page of Project Settings.


#include "FreindUIWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
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
            // 친구 초대 여기에서
        }
    }
}

void UFreindUIWidget::OnCloseClicked()
{
    OnCloseRequested();
}