// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Name_Widget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API UName_Widget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // 성 입력창
    UPROPERTY(meta = (BindWidget))
    class UEditableText* LastNameBox;

    // 이름 입력창
    UPROPERTY(meta = (BindWidget))
    class UEditableText* FirstNameBox;

    // 확인 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* OK_BTN;

    UFUNCTION()
    void OnOKClicked();
};
