// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "Virtual_life_GameInstance.h"


UQuest::UQuest()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/Quest_system/Quest_Data_Table.Quest_Data_Table"));
    if (DataTableFinder.Succeeded())
    {
        QuestDataTable = DataTableFinder.Object;
    }

}

void UQuest::init_quest(const uint8& Quest_ID)
{


    
        FString RowName = FString::FromInt(Quest_ID);
        FQuestDataRow* RowData = QuestDataTable->FindRow<FQuestDataRow>(FName(*RowName), TEXT("Quest Lookup"));
        if (RowData)
        {
            Quest_info = *RowData;

            if (!Quest_info.IsMain) {
                //게임인스턴스에서 인벤토리를 가져옴
                UVirtual_life_GameInstance* game_inst = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
                for (const auto& r : Quest_info.RequirValues) {
                    Goals.Add(requir(r.Item_ID,r.RequirNum, &game_inst->m_inventory->Owned_Items[r.Item_ID]));
                }
            }

        }

    
    
	
}

bool UQuest::quest_success()
{
    bool all_complete = true;
    for (const auto& a : Goals) all_complete = all_complete && (*a.have_num >= a.requir_num);
    return all_complete;
}
