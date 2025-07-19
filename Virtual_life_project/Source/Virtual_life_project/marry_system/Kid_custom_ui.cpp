#include "Kid_custom_ui.h"
#include "Components/Slider.h" 
#include "Components/VerticalBox.h" 
#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Math/UnrealMathUtility.h"

void UKid_custom_ui::NativeConstruct()
{
    Super::NativeConstruct();

    if (Genetic_ratio_slider)
    {
        Genetic_ratio_slider->SetValue(0.5f);
    }

    // 체크박스 이벤트 바인딩
    if (Personality_Check1)
    {
        Personality_Check1->OnCheckStateChanged.AddDynamic(this, &UKid_custom_ui::_cheak1);
    }
    if (Personality_Check2)
    {
        Personality_Check2->OnCheckStateChanged.AddDynamic(this, &UKid_custom_ui::_cheak2);
    }
    if (Personality_Check3)
    {
        Personality_Check3->OnCheckStateChanged.AddDynamic(this, &UKid_custom_ui::_cheak3);
    }
    if (Personality_Check4)
    {
        Personality_Check4->OnCheckStateChanged.AddDynamic(this, &UKid_custom_ui::_cheak4);
    }

    if (OK_button)
    {
        OK_button->OnClicked.AddDynamic(this, &UKid_custom_ui::on_ok_btn);
    }

    if (random_genetic) {
        random_genetic->OnCheckStateChanged.AddDynamic(this, &UKid_custom_ui::random_gentic_cheak);
    }
}

void UKid_custom_ui::NativeDestruct()
{
    Super::NativeDestruct();
}

void UKid_custom_ui::all_uncheak()
{
    if (Personality_Check1) Personality_Check1->SetIsChecked(false);
    if (Personality_Check2) Personality_Check2->SetIsChecked(false);
    if (Personality_Check3) Personality_Check3->SetIsChecked(false);
    if (Personality_Check4) Personality_Check4->SetIsChecked(false);
}

void UKid_custom_ui::random_gentic_cheak(bool bIsChecked)
{
    if (bIsChecked)
    {
        if (Genetic_ratio_box) Genetic_ratio_box->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        if (Genetic_ratio_box) Genetic_ratio_box->SetVisibility(ESlateVisibility::Visible);
    }
}

void UKid_custom_ui::_cheak1(bool bIsChecked)
{
    if (bIsChecked)
    {
        all_uncheak();
        if (Personality_Check1) Personality_Check1->SetIsChecked(true);
    }
    else  Personality_Check1->SetIsChecked(true);
}

void UKid_custom_ui::_cheak2(bool bIsChecked)
{
    if (bIsChecked)
    {
        all_uncheak();
        if (Personality_Check2) Personality_Check2->SetIsChecked(true);
    }
    else Personality_Check2->SetIsChecked(true);
}

void UKid_custom_ui::_cheak3(bool bIsChecked)
{
    if (bIsChecked)
    {
        all_uncheak();
        if (Personality_Check3) Personality_Check3->SetIsChecked(true);
    }
    else Personality_Check3->SetIsChecked(true);
}

void UKid_custom_ui::_cheak4(bool bIsChecked)
{
    if (bIsChecked)
    {
        all_uncheak();
        if (Personality_Check4) Personality_Check4->SetIsChecked(true);
    }
    else Personality_Check4->SetIsChecked(true);
}

void UKid_custom_ui::on_ok_btn()
{
    float ProgressValue = Genetic_ratio_slider && (!random_genetic || !random_genetic->IsChecked()) ? Genetic_ratio_slider->GetValue() : 0.5f;
    uint8 Personality = FMath::RandRange(0, 2);  // 기본값
    FString InputText = hello_etb ? hello_etb->GetText().ToString() : TEXT("");
    FString name_input = name_etb ? name_etb->GetText().ToString() : TEXT("KID");

    // 선택된 체크박스에 따라 Personality 값 설정
    if (Personality_Check1 && Personality_Check1->IsChecked())
    {
        Personality = FMath::RandRange(0, 2); 
    }
    else if (Personality_Check2 && Personality_Check2->IsChecked())
    {
        Personality = 0; 
    }
    else if (Personality_Check3 && Personality_Check3->IsChecked())
    {
        Personality = 1;
    }
    else if (Personality_Check4 && Personality_Check4->IsChecked())
    {
        Personality = 2; 
    }

    // 델리게이트 호출
    
    OnKidSetting.Broadcast(ProgressValue, Personality, InputText,name_input);
}