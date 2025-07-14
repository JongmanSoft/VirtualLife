// Fill out your copyright notice in the Description page of Project Settings.


#include "Room_Join_Widget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Virtual_life_GameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void URoom_Join_Widget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Join_BTN)
    {
        Join_BTN->OnClicked.AddDynamic(this, &URoom_Join_Widget::OnButtonClicked);
    }
}

void URoom_Join_Widget::OnButtonClicked()
{
    if (Join_ID)
    {
        FString EnteredID = Join_ID->GetText().ToString();

        if (!EnteredID.IsEmpty())
        {
            UVirtual_life_GameInstance* my_instance = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
            if (my_instance)
            {
                my_instance->SendEnterRoom(EnteredID);
            }
        }
    }
}