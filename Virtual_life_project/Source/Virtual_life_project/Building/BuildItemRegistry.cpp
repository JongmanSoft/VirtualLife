// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildItemRegistry.h"
#include "Virtual_life_GameInstance.h"

TMap<FName, uint16> FBuildItemRegistry::NameToIDMap;
TMap<uint16, FName> FBuildItemRegistry::IDToNameMap;

void FBuildItemRegistry::Initialize(UDataTable* InDataTable)
{
    NameToIDMap.Empty();
    IDToNameMap.Empty();

    TArray<FName> RowNames = InDataTable->GetRowNames();
    for (uint16 i = 0; i < RowNames.Num(); ++i)
    {
        FName RowName = RowNames[i];
        NameToIDMap.Add(RowName, i);
        IDToNameMap.Add(i, RowName);
    }
}

uint16 FBuildItemRegistry::FNameToItemID(const FName& Name)
{
    if (NameToIDMap.Contains(Name))
    {
        return NameToIDMap[Name];
    }

    return UINT16_MAX;
}

FName FBuildItemRegistry::ItemIDToFName(uint16 ID)
{
    if (IDToNameMap.Contains(ID))
    {
        return IDToNameMap[ID];
    }

    return NAME_None;
}

const FBuildInfo* FBuildItemRegistry::FindBuildInfo(FName RowID)
{
    if (!GWorld) return nullptr;
    auto GI = Cast<UVirtual_life_GameInstance>(UGameplayStatics::GetGameInstance(GWorld));
    if (!GI || !GI->BuildingDataTable) return nullptr;
    return GI->BuildingDataTable->FindRow<FBuildInfo>(RowID, TEXT("FindBuildInfo"));

}
