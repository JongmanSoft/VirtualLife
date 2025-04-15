// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingHUBWidget.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/ScrollBox.h"
#include "BuildingPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingSelectButtonWidget.h"

void UBuildingHUBWidget::NativeConstruct()
{
    Super::NativeConstruct();
    BindTabButtons();
}

void UBuildingHUBWidget::BindTabButtons()
{
    if (FurnitureTab_BTN)
        FurnitureTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnFurnitureTabClicked);

    if (BedroomTab_BTN)
        BedroomTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnBedroomTabClicked);

    if (KitchenTab_BTN)
        KitchenTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnKitchenTabClicked);

    if (StorageTab_BTN)
        StorageTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnStorageTabClicked);

    if (InteriorDecorTab_BTN)
        InteriorDecorTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnInteriorDecorTabClicked);

    if (DecorTab_BTN)
        DecorTab_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnDecorTabClicked);

    if (Confirm_BTN)
        Confirm_BTN->OnClicked.AddDynamic(this, &UBuildingHUBWidget::OnConfirmClicked);
}

void UBuildingHUBWidget::OnCategorySelected(EBuildCategories Category)
{
    if (!BuildingDataTable || !WrapBox_Buildings) return;

    WrapBox_Buildings->ClearChildren();

    for (const FName& RowName : BuildingDataTable->GetRowNames())
    {
        const FBuildInfo* Info = BuildingDataTable->FindRow<FBuildInfo>(RowName, "");
        if (Info && Info->Categories == Category)
        {
            UBuildingSelectButtonWidget* Button = CreateWidget<UBuildingSelectButtonWidget>(this, ButtonWidgetClass);
            if (Button)
            {
                Button->SetRowData(BuildingDataTable, RowName); // 커스텀 함수 필요
                WrapBox_Buildings->AddChild(Button);
            }
        }
    }
}

void UBuildingHUBWidget::OnFurnitureTabClicked()
{
    OnCategorySelected(EBuildCategories::Furniture);
}

void UBuildingHUBWidget::OnBedroomTabClicked()
{
    OnCategorySelected(EBuildCategories::Bedroom);
}

void UBuildingHUBWidget::OnKitchenTabClicked()
{
    OnCategorySelected(EBuildCategories::Kitchen);
}

void UBuildingHUBWidget::OnStorageTabClicked()
{
    OnCategorySelected(EBuildCategories::Storage);
}

void UBuildingHUBWidget::OnInteriorDecorTabClicked()
{
    OnCategorySelected(EBuildCategories::InteriorDecor);
}

void UBuildingHUBWidget::OnDecorTabClicked()
{
    OnCategorySelected(EBuildCategories::Decor);
}

void UBuildingHUBWidget::OnConfirmClicked()
{
    ABuildingPlayerController* PC = Cast<ABuildingPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PC) return;

    PC->ConfirmBuildPlacement();

    UE_LOG(LogTemp, Warning, TEXT("확정 버튼 눌림! 건물 정보 전송."));
}
