// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Virtual_life_GameInstance.h"
#include "Quset_ui.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API UQuset_ui : public UUserWidget
{
    GENERATED_BODY()
public:
    // 퀘스트 ID로 위젯 초기화
    void InitializeWithQuestID(uint8 QuestID);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnInventoryUpdated(const uint8& ItemID);


protected:
    void UpdateQuestDisplay(); // 내부 UI 갱신 함수

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<uint8> use_Item_ID; //사용중인 아이템아이디

    UPROPERTY()
    UQuest* QuestRef;  //퀘스트 래퍼런스


    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UTextBlock* QuestNameText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UTextBlock* QuestRequiredValue;

};
