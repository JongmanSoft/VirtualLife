// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest.generated.h"


struct FWriteRequirValue //실제 퀘스트 클래스 내에서 사용하는 구조체
{
    int32* RequirBindValue;
    uint8 RequirCount;
};

struct FReward_value //리워드 구조체
{
    int32* RewardValue;
    uint8 RewardCount;
};

USTRUCT(BlueprintType)
struct FReadRequirValue //블루프린트에서도 쓸수있는 출력을 위한 구조체, 흠 최적화가 안될거같은데......
{
    GENERATED_BODY()

public:
    int32 RequirBindValue; 

    UPROPERTY(BlueprintReadWrite)
    uint8 RequirCount =0; 
};

UCLASS(Blueprintable)
class VIRTUAL_LIFE_PROJECT_API UQuest : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    FString QuestName; 

    UPROPERTY(BlueprintReadWrite)
    FString QuestDesc;

    TArray<FWriteRequirValue> RequirValues;

    TArray<FReward_value> RewardValues;


    bool QuestComplete();



    UFUNCTION(BlueprintCallable)
    TArray<FReadRequirValue> GetRequirValues() const;
};