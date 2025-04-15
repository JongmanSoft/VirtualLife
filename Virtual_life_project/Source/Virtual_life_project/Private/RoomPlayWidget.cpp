// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPlayWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void URoomPlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EnterBuildModeButton)
		EnterBuildModeButton->OnClicked.AddDynamic(this, &URoomPlayWidget::OnEnterBuildModeClicked);

	if (QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &URoomPlayWidget::OnQuitClicked);
}

void URoomPlayWidget::OnEnterBuildModeClicked()
{
	
}

void URoomPlayWidget::OnQuitClicked()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("OpenWorldMap")));
}