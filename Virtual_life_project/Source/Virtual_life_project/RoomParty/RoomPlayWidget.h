// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomPlayWidget.generated.h"

/**
 * 
 */
class UMyProfileWidget;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API URoomPlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:


	UPROPERTY(meta = (BindWidget))
	UMyProfileWidget* MyProfile_UI;

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* EnterBuildModeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* edit_text;

	UPROPERTY()
	bool is_my_room = false;

private:
	UFUNCTION()
	void OnEnterBuildModeClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION(BlueprintCallable)
	void disable_enter_building();


};
