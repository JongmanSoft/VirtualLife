// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "Virtual_life_GameInstance.h"


UQuest::UQuest()
{

}

UQuest::UQuest(const uint8& Quest_ID)
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/Path/To/Your/QuestDataTable"));

    if (DataTableFinder.Succeeded())
    {
        UDataTable* QuestDataTable = DataTableFinder.Object;
        FString RowName = FString::FromInt(Quest_ID);
        FQuestDataRow* RowData = QuestDataTable->FindRow<FQuestDataRow>(FName(*RowName), TEXT("Quest Lookup"));
        if (RowData)
        {
            Quest_info = *RowData;

            if (!Quest_info.IsMain) {
                for (auto r : Quest_info.RequirValues) {
                    
                }
            }

        }

    }
    
	
}
