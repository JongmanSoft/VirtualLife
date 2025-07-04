// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPlayerController.h"
#include "Virtual_life_GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"


void ABuildingPlayerController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void ABuildingPlayerController::AddPendingBuild(const FObjectData& Data)
{
    PendingBuildObjects.Add(Data);
}

void ABuildingPlayerController::RemovePendingBuildAtLocation(const FVector& Location, float Tolerance)
{
    PendingBuildObjects.RemoveAll([&](const FObjectData& Obj) {
        return FVector::Dist(Obj.Location, Location) <= Tolerance;
        });
}

void ABuildingPlayerController::ConfirmBuildPlacement()
{
    auto GI = Cast<UVirtual_life_GameInstance>(GetGameInstance());
    if (!GI) return;

    GI->SendPlaceBuildPacket(PendingBuildObjects);

    PendingBuildObjects.Empty();
}

void ABuildingPlayerController::TrySelectBuildActor()
{
    FHitResult Hit;

    if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Hit))
    {
        AActor* HitActor = Cast<AActor>(Hit.GetActor());
        if (!HitActor) return;

        if (HitActor->IsA(APlaceBuildActor::StaticClass()) || HitActor->IsA(AInteractableActor::StaticClass()))
        {
            SelectedBuildActor = HitActor;

            UE_LOG(LogTemp, Warning, TEXT("클릭된 액터: %s"), *HitActor->GetName());

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
