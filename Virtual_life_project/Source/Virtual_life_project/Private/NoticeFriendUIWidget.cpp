// Fill out your copyright notice in the Description page of Project Settings.


#include "NoticeFriendUIWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

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

void UNoticeFriendUIWidget::OnYesClicked()
{
    // 파티 수락
    RemoveFromParent();
}

void UNoticeFriendUIWidget::OnNoClicked()
{
    // 거절
    RemoveFromParent();
}
