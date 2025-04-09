// Quest_DataTable_Row.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Quest_DataTable_Row.generated.h"

USTRUCT(BlueprintType)
struct FRequirItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    uint8 Item_ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    uint8 RequirNum;
};

USTRUCT(BlueprintType)
struct FQuestDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    bool IsMain = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString Quest_Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FString Quest_Desc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<FRequirItem> RequirValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    int32 RewardGold;
};