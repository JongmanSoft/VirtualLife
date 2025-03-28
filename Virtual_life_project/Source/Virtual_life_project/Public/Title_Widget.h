// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Title_Widget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API UTitle_Widget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* NewStart_BTN; // 새로 시작하기

	UPROPERTY(meta = (BindWidget))
	class UButton* Start_BTN; // 이어 하기

	UPROPERTY(meta = (BindWidget))
	class UButton* Quit_BTN; // 게임 종료

	UPROPERTY(meta = (BindWidget))
	class UButton* Option_BTN; // 설정

	// 이벤트 함수
	UFUNCTION()
	void OnNewStartClicked();

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnOptionClicked();

};
