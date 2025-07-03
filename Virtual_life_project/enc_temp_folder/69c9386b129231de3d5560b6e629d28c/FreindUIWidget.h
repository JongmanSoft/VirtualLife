// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FreindUIWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UEditableTextBox;
class UButton;

UCLASS()
class UFreindUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* Invite_ID;

    UPROPERTY(meta = (BindWidget))
    UButton* Invite_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* Close_BTN;

    UFUNCTION(BlueprintImplementableEvent)
    void OnCloseRequested();

    UFUNCTION(BlueprintCallable)
    void OnCloseClicked();

private:
    // 버튼 콜백 함수
    UFUNCTION()
    void OnInviteClicked();
};
