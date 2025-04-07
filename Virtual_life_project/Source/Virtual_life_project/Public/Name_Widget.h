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

    // 전체 이름 출력 텍스트 블록
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UTextBlock* FullNameTXT;

    // 성 입력창
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UEditableTextBox* LastNameBox;

    // 이름 입력창
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UEditableTextBox* FirstNameBox;

    // 확인 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* OK_BTN;

    // 돌아가기 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* Back_BTN;

    UFUNCTION()
    void OnOKClicked();

    UFUNCTION()
    void OnBackClicked();

    UFUNCTION()
    void OnLastNameBoxFocused(const FText& Text, ETextCommit::Type CommitMethod);

    UFUNCTION()
    void OnFirstNameBoxFocused(const FText& Text, ETextCommit::Type CommitMethod);

    UFUNCTION()
    void UpdateFullNameText();
};
