// Fill out your copyright notice in the Description page of Project Settings.


#include "Quset_ui.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"



void UQuset_ui::InitializeWithQuestID(uint8 QuestID)
{

    if (UVirtual_life_GameInstance* GameInstance = Cast<UVirtual_life_GameInstance>(GetGameInstance()))
    {
        QuestRef = GameInstance->m_quest->Quests[QuestID];

        for (const auto& a : QuestRef->Goals) {
            use_Item_ID.Add(a.requir_item_ID);
        }
    }
}

void UQuset_ui::NativeConstruct()
{
    Super::NativeConstruct();

    Item_Datatable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Item_system/ITEM_DATA_Table.ITEM_DATA_Table"));

    if (UVirtual_life_GameInstance* GameInstance = Cast<UVirtual_life_GameInstance>(GetGameInstance()))
    {

        GameInstance->OnInventoryChanged.AddDynamic(this, &UQuset_ui::OnInventoryUpdated);
        QuestRef->OnChangeQuestComplete.AddDynamic(this ,&UQuset_ui::UpdateQuestDisplay);
        // 초기화된 QuestID로 UI 갱신
        UpdateQuestDisplay();
    }
}

void UQuset_ui::NativeDestruct()
{
    if (UVirtual_life_GameInstance* GameInstance = Cast<UVirtual_life_GameInstance>(GetGameInstance()))
    {
        //구독 끊겠습니다...
        GameInstance->OnInventoryChanged.RemoveDynamic(this, &UQuset_ui::OnInventoryUpdated);
        QuestRef->OnChangeQuestComplete.RemoveDynamic(this, &UQuset_ui::UpdateQuestDisplay);
    }
    Super::NativeDestruct();
}

void UQuset_ui::OnInventoryUpdated(const uint8& ItemID)
{
    if (use_Item_ID.Contains(ItemID))UpdateQuestDisplay();
}

void UQuset_ui::UpdateQuestDisplay()
{
    if (QuestRef)
    {
        //완료이미지 셋팅
        if (QuestRef->quest_success()) {
            cheak_image->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Quest_UI/complete.complete")));
        }
        else cheak_image->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/Quest_UI/not_complete.not_complete")));


        QuestNameText->SetText(FText::FromString(QuestRef->Quest_info.Quest_Name));

        FString GoalsText;

        if (QuestRef->Quest_info.IsMain) {
            QuestRequiredValue->SetText(FText::FromString(TEXT(" ")));
            return;
        }

       
   
        for (const requir& Goal : QuestRef->Goals)
        {
            
            FString item_Name = "";
            if (Item_Datatable) {
                FString RowName = FString::FromInt(Goal.requir_item_ID);
                FItemData* RowData = Item_Datatable->FindRow<FItemData>(FName(*RowName), TEXT("Quest Lookup"));
                item_Name = RowData->ItemName;
            }
            GoalsText += FString::Printf(TEXT("%s  (%d / %d)\n"),
                *item_Name, *Goal.have_num, Goal.requir_num);
        }
        QuestRequiredValue->SetText(FText::FromString(GoalsText));
    }
}
