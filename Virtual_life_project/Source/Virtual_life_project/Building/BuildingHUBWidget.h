// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FBuildInfo.h"
#include "PlacementActor.h"
#include "BuildingHUBWidget.generated.h"

class UButton;
class UWrapBox;
class UScrollBox;
class UBuildingSelectButtonWidget;
class UMyProfileWidget;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UBuildingHUBWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable)
    void OnCategorySelected(EBuildCategories Category);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<APlacementActor> PlacementActorClass;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    UDataTable* BuildingDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UBuildingSelectButtonWidget> ButtonWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* ScrollBox_BuildingList;

    UPROPERTY(meta = (BindWidget))
    UWrapBox* WrapBox_Buildings;

    // 우리 테마 버튼
    UPROPERTY(meta = (BindWidget))
    UButton* MinjiTheme_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* SeyoungTheme_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* HaenimTheme_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* SaveTheme_BTN;

    void ExportCurrentThemeToJson(const FString& FileName);
    void LoadThemeFromJson(const FString& FileName);

    // 메뉴 탭들
    UPROPERTY(meta = (BindWidget))
    UButton* FurnitureTab_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* BedroomTab_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* KitchenTab_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* StorageTab_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* InteriorDecorTab_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* DecorTab_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* StructureTab_BTN;

    UPROPERTY(meta = (BindWidget))
    UButton* Confirm_BTN;

    UPROPERTY(meta = (BindWidget))
    UMyProfileWidget* MyProfile_UI;

private:
    void BindTabButtons();

    UFUNCTION()
    void OnMJBTNClicked();

    UFUNCTION()
    void OnSYBTNClicked();

    UFUNCTION()
    void OnHNBTNClicked();

    UFUNCTION()
    void OnSaveThemeClicked();

    UFUNCTION()
    void OnFurnitureTabClicked();

    UFUNCTION()
    void OnBedroomTabClicked();

    UFUNCTION()
    void OnKitchenTabClicked();

    UFUNCTION()
    void OnStorageTabClicked();

    UFUNCTION()
    void OnInteriorDecorTabClicked();

    UFUNCTION()
    void OnDecorTabClicked();

    UFUNCTION()
    void OnStructureTabClicked();

    UFUNCTION()
    void OnConfirmClicked();
};
