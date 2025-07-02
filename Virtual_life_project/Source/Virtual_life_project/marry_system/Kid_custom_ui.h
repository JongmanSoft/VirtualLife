#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h" // UEditableTextBox 추가
#include "Kid_custom_ui.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnKidSetting, float, ProgressValue, uint8, Personality, FString, InputText);

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UKid_custom_ui : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:
    // 델리게이트 선언
    UPROPERTY(BlueprintAssignable, Category = "Delegate")
    FOnKidSetting OnKidSetting;

protected:
    // 프로그레스바
    UPROPERTY(meta = (BindWidget))
    class USlider* Genetic_ratio_slider;
    
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* Genetic_ratio_box;

    //보간 자동으로 할지말쥐
    UPROPERTY(meta = (BindWidget))
    class UCheckBox* random_genetic;

    // 라디오 버튼 (4가지 선택지 중 택1)
    UPROPERTY(meta = (BindWidget))
    class UCheckBox* Personality_Check1;

    UPROPERTY(meta = (BindWidget))
    class UCheckBox* Personality_Check2;

    UPROPERTY(meta = (BindWidget))
    class UCheckBox* Personality_Check3;

    UPROPERTY(meta = (BindWidget))
    class UCheckBox* Personality_Check4;

    // 인사말 입력
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* hello_etb;

    // 결정 버튼
    UPROPERTY(meta = (BindWidget))
    class UButton* OK_button;

    UFUNCTION()
    void random_gentic_cheak(bool bIsChecked);

    // 체크박스 이벤트 핸들러
    UFUNCTION()
    void _cheak1(bool bIsChecked);

    UFUNCTION()
    void _cheak2(bool bIsChecked);

    UFUNCTION()
    void _cheak3(bool bIsChecked);

    UFUNCTION()
    void _cheak4(bool bIsChecked);


    // OK 버튼 클릭 핸들러
    UFUNCTION()
    void on_ok_btn();

    // 모든 체크박스 해제
    void all_uncheak();
};