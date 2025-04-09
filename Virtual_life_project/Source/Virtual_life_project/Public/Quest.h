// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest_DataTable_Row.h"
#include "Quest.generated.h"

struct requir
{
    uint8 requir_item_ID;
    uint8 requir_num;
    int32* have_num;
};

UCLASS(Blueprintable)
class VIRTUAL_LIFE_PROJECT_API UQuest : public UObject
{
    GENERATED_BODY()

public:
 
    //퀘스트 정보
    FQuestDataRow Quest_info;
    //현재 진행정보
    
public:
    UQuest();
    UQuest(const uint8& Quest_ID); //아이디를 주면 맞는퀘스트생성
 
    TArray<requir> Goals;

 

};