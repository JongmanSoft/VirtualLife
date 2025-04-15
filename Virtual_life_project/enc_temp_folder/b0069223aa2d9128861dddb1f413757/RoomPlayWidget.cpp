// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPlayWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "BuildingPlayerController.h"


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

	ULocalPlayer* LocalPlayer = World->GetFirstLocalPlayerFromController();
	if (!LocalPlayer || !BuildingPawnClass || !BuildingControllerClass) return;

	APlayerController* OldController = LocalPlayer->PlayerController;
	APawn* OldPawn = OldController ? OldController->GetPawn() : nullptr;

	if (OldPawn)
	{
		OldController->UnPossess();

		// OldPawn->Destroy();

		// 见扁扁 + 厚劝己拳
		OldPawn->SetActorHiddenInGame(true);
		OldPawn->SetActorEnableCollision(false);
		OldPawn->SetActorTickEnabled(false);
	}

	// 货 Pawn & Controller 积己
	FVector SpawnLoc(-2400.000366f, -7139.999878f, 4390.0f);
	FRotator SpawnRot(0.f, -40.f, 0.f);

	RemoveFromParent();

	APawn* NewPawn = World->SpawnActor<APawn>(BuildingPawnClass, SpawnLoc, SpawnRot);
	if (!NewPawn) return;

	APlayerController* NewController = World->SpawnActor<APlayerController>(BuildingControllerClass, SpawnLoc, SpawnRot);
	if (!NewController) return;

	LocalPlayer->SwitchController(NewController);

	NewController->Possess(NewPawn);
}

void URoomPlayWidget::OnQuitClicked()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("OpenWorldMap")));
}