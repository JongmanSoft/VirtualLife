// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "FBuildInfo.h"
#include "BuildingSelectButtonWidget.generated.h"

class APlacementActor;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UBuildingSelectButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void CheckActive();
    virtual void CheckActive_Implementation();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<APlacementActor> PlacementActorClass;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    UDataTable* DataTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta = (GetOptions = "GetRowNames"))
    FName RowName;

    UPROPERTY(meta = (BindWidget))
    class UImage* AssetImage;

    UPROPERTY(meta = (BindWidget))
    class UButton* Build_BTN;

    UFUNCTION()
    void OnClickedBuildButton();

    UFUNCTION()
    TArray<FName> GetRowNames() const
    {
        if (!DataTable) return {};

        return DataTable->GetRowNames();
    }
public:
    UFUNCTION(BlueprintCallable)
    void SetRowData(UDataTable* InDataTable, FName InRowName);
};
