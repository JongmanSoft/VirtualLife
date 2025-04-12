// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest_Manager.h"

void UQuest_Manager::ADD_QUEST(const uint8& quest_id)
{
    // NewObject를 사용해 UQuest 객체 생성
    UQuest* NewQuest = NewObject<UQuest>(this, UQuest::StaticClass());
    NewQuest->init_quest(quest_id);
    if (NewQuest)
    {
        Quests.Add(quest_id, NewQuest);
    }
}

bool UQuest_Manager::Quest_complete(const uint8& quest_id)
{
    if (!Quests.Contains(quest_id)) return false;
    return Quests[quest_id]->quest_success();
}

void UQuest_Manager::Delete_Quest(const uint8& quest_id)
{
    Quests.Remove(quest_id);
}
