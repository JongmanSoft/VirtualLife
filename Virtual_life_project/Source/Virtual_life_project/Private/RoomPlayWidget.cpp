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
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* OldController = UGameplayStatics::GetPlayerController(World, 0);
	if (!OldController || !BuildingPawnClass || !BuildingControllerClass) return;

	FVector SpawnLoc(-2400.000366f, -7139.999878f, 4390.0f);
	FRotator SpawnRot(0.f, -40.f, 0.f);

	RemoveFromParent();

	APawn* NewPawn = World->SpawnActor<APawn>(BuildingPawnClass, SpawnLoc, SpawnRot);
	if (!NewPawn) return;

	APlayerController* NewController = World->SpawnActor<APlayerController>(BuildingControllerClass, SpawnLoc, SpawnRot);
	if (!NewController) return;

	NewController->Possess(NewPawn);

	if (BuildingUIClass)
	{
		UUserWidget* NewUI = CreateWidget<UUserWidget>(NewController, BuildingUIClass);
		if (NewUI)
		{
			NewUI->AddToViewport();
		}
	}
}

void URoomPlayWidget::OnQuitClicked()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("OpenWorldMap")));
}