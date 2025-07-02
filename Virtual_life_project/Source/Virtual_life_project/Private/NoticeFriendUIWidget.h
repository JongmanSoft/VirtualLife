// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoticeFriendUIWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;

UCLASS()
class UNoticeFriendUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Friend_ID;

    UPROPERTY(meta = (BindWidget))
    UButton* YesBTN;

    UPROPERTY(meta = (BindWidget))
    UButton* NoBTN;

private:
    UFUNCTION()
    void OnYesClicked();

    UFUNCTION()
    void OnNoClicked();
};
