// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest_DataTable_Row.h"
#include "Quest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeQuestComplete);

struct requir
{
    uint8 requir_item_ID;
    uint8 requir_num;
    int* have_num;
};

UCLASS(Blueprintable)
class VIRTUAL_LIFE_PROJECT_API UQuest : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
    FOnChangeQuestComplete OnChangeQuestComplete;

public:
 //데이터 테이블
    UDataTable* QuestDataTable;
    //퀘스트 정보
    FQuestDataRow Quest_info;
    //현재 진행정보
    TArray<requir> Goals;
    //완료를 나타내는 bool
    bool complete;
public:
    UQuest();
    void init_quest(const uint8& Quest_ID); //아이디를 주면 맞는퀘스트생성
    bool quest_success();
    void set_complete(const bool& com);
};