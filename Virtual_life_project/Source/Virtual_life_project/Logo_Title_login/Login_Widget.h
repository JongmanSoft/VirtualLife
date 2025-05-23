// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Login_Widget.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API ULogin_Widget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    // ID 입력창
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* ID_ETB;

    // PW 입력창
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* Password_ETB;

    // 로그인 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* Login_Btn;

    UFUNCTION()
    void OnLoginClicked();

    UFUNCTION()
    void OnIDTextChanged(const FText& Text);
};
