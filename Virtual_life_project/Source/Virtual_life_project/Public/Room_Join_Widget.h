// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Room_Join_Widget.generated.h"

/**
 * 
 */

class UTextBlock;
class UEditableTextBox;
class UButton;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API URoom_Join_Widget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;


public:
    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* Join_ID;

    UPROPERTY(meta = (BindWidget))
    UButton* Join_BTN;

private:
    // 버튼 콜백 함수
    UFUNCTION()
    void OnButtonClicked();

};

