#include "PlayerInventory.h"
// Fill out your copyright notice in the Description page of Project Settings.
#include "Virtual_life_GameInstance.h"

UPlayerInventory::UPlayerInventory()
{
    Ownerd_Equip.SetNum(5);
}

void UPlayerInventory::Add_Item(uint8 Add_ID, uint8 Add_num)
{
    if (Owned_Items.Contains(Add_ID))
    {
        Owned_Items[Add_ID] += Add_num;
    }
    else
    {
        Owned_Items.Add(Add_ID, Add_num);
    }
}

bool UPlayerInventory::Delete_Item(uint8 Add_ID, uint8 Add_num)
{
    if (!Owned_Items.Contains(Add_ID) || Owned_Items[Add_ID] < Add_num)return false;
    else {
        Owned_Items[Add_ID] -= Add_num;
        if (Owned_Items[Add_ID] <= 0) Owned_Items.Remove(Add_ID);
        return true;
    }
}

void UPlayerInventory::road_Item(TMap<uint8, uint8> New_Items)
{
    Owned_Items = New_Items;
}

void UPlayerInventory::get_item(uint8 id, uint8 num) 
{
    UWorld* World = GetWorld();
    if (World)
    {
        UVirtual_life_GameInstance* GameInstance = Cast<UVirtual_life_GameInstance>(World->GetGameInstance());
        if (GameInstance) {
            GameInstance->SendGetItemPacket(id, num);
        }
    }
}

void UPlayerInventory::update_item(uint8 id, uint8 num)
{
    Owned_Items[id] = num;
}

void UPlayerInventory::delete_zero_item()
{
for(auto i:Owned_Items)if (i.Value <= 0) Owned_Items.Remove(i.Key);
}
