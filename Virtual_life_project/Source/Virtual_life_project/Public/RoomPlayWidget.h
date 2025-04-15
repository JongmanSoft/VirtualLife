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

	// 클래스 참조들 (에디터에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSubclassOf<APawn> BuildingPawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSubclassOf<APlayerController> BuildingControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSubclassOf<UUserWidget> BuildingUIClass;

private:
	UFUNCTION()
	void OnEnterBuildModeClicked();

	UFUNCTION()
	void OnQuitClicked();
};
