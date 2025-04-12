// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPlayerController.h"
#include "Blueprint/UserWidget.h"


void ABuildingPlayerController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void ABuildingPlayerController::TrySelectBuildActor()
{
    FHitResult Hit;
    if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Hit))
    {
        APlaceBuildActor* HitActor = Cast<APlaceBuildActor>(Hit.GetActor());
        if (HitActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("클릭된 액터: %s"), *HitActor->GetName());

            SelectedBuildActor = HitActor;

            if (InteractionWidgetClass)
            {
                UUserWidget* Widget = CreateWidget<UUserWidget>(this, InteractionWidgetClass);
                if (Widget)
                {
                    Widget->AddToViewport();
                }
            }
        }
    }
}
