// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FBuildInfo.h"


/**
 * 
 */
class VIRTUAL_LIFE_PROJECT_API FBuildItemRegistry
{
public:
    static void Initialize(UDataTable* InDataTable);

    static uint16 FNameToItemID(const FName& Name);
    static FName ItemIDToFName(uint16 ID);

    static const FBuildInfo* FindBuildInfo(FName RowID);

private:
    static TMap<FName, uint16> NameToIDMap;
    static TMap<uint16, FName> IDToNameMap;
};

