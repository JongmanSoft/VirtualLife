// Fill out your copyright notice in the Description page of Project Settings.


#include "party_leave_button.h"
#include "Virtual_life_GameInstance.h"

void Uparty_leave_button::NativeConstruct()
{
    Super::NativeConstruct();
    if (YesBTN)
    {
        YesBTN->OnClicked.AddDynamic(this, &Uparty_leave_button::party_leave_ok);
    }
}

void Uparty_leave_button::party_leave_ok()
{
    UVirtual_life_GameInstance* my_instance = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
    if (my_instance)
    {
        my_instance->SendPartyLeavePacket();
    }

    RemoveFromParent();
}
