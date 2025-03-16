// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest_Manager.h"


void UQuest_Manager::Add_Quest_Single(FString Quest_name, FString Quest_desc, int32& requir_value, int32 requir_count, int32& RewardValue, int32 RewardCount)
{
    UQuest* temp = NewObject<UQuest>();
    temp->QuestName = Quest_name;
    temp->QuestDesc = Quest_desc;
    temp->RequirValues.Add(FWriteRequirValue(&requir_value, requir_count));
    temp->RewardValues.Add(FReward_value(&RewardValue, RewardCount));
    Quests.Add(temp);
}

void UQuest_Manager::Add_Quest_Multi(FString Quest_name, FString Quest_desc, int32& requir_value, TArray<FWriteRequirValue> new_requir, TArray<FReward_value> new_reward)
{
    UQuest* temp = NewObject<UQuest>();
    temp->QuestName = Quest_name;
    temp->QuestDesc = Quest_desc;
    temp->RequirValues =new_requir;
    temp->RewardValues=new_reward;
    Quests.Add(temp);
}
