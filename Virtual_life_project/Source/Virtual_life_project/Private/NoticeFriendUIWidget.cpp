// Fill out your copyright notice in the Description page of Project Settings.


#include "NoticeFriendUIWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Virtual_life_GameInstance.h"

void UNoticeFriendUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (YesBTN)
    {
        YesBTN->OnClicked.AddDynamic(this, &UNoticeFriendUIWidget::OnYesClicked);
    }

    if (NoBTN)
    {
        NoBTN->OnClicked.AddDynamic(this, &UNoticeFriendUIWidget::OnNoClicked);
    }
}

void UNoticeFriendUIWidget::SetText(FString str)
{
    Friend_ID->SetText(FText::FromString(str));
}

void UNoticeFriendUIWidget::OnYesClicked()
{
    // 파티 수락
    UVirtual_life_GameInstance* my_instance = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
    if (my_instance)
    {
        my_instance->SendPartyJoinPacket(FriendID);
    }

    RemoveFromParent();
}

void UNoticeFriendUIWidget::OnNoClicked()
{
    // 거절
    UVirtual_life_GameInstance* my_instance = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
    if (my_instance)
    {
        my_instance->SendPartyRejectPacket(FriendID);
    }

    RemoveFromParent();
}
