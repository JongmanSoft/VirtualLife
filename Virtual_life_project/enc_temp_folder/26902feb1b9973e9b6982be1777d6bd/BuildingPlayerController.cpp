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

void ABuildingPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    UE_LOG(LogTemp, Warning, TEXT("[Controller] OnPossess 호출됨!"));

    SetupInputMapping();
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

void ABuildingPlayerController::SetupInputMapping()
{

    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (!LocalPlayer)
    {
        UE_LOG(LogTemp, Error, TEXT("[Controller] LocalPlayer 없음!"));
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    if (Subsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Controller] Input Subsystem 찾음"));

        if (BuildingMappingContext)
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(BuildingMappingContext, 0);
            // UE_LOG(LogTemp, Warning, TEXT("[Controller] MappingContext 등록 완료"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Controller] BuildingMappingContext 없음!"));
        }
    }

    if (BuildingUIClass && IsLocalController())
    {
        if (!BuildingUI)
        {
            BuildingUI = CreateWidget<UUserWidget>(this, BuildingUIClass);
        }

        if (BuildingUI && !BuildingUI->IsInViewport())
        {
            BuildingUI->AddToViewport();
        }
    }
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
