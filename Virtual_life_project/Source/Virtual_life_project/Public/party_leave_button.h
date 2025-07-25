// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "party_leave_button.generated.h"

/**
 * 
 */



UCLASS()
class VIRTUAL_LIFE_PROJECT_API Uparty_leave_button : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UButton* YesBTN;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void party_leave_ok();
};
