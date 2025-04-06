// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSelectButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "PlacementActor.h"

void UBuildingSelectButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Build_BTN)
    {
        Build_BTN->OnClicked.AddDynamic(this, &UBuildingSelectButtonWidget::OnBuildButtonClicked);
    }

    if (DataTable && AssetImage)
    {
        const FBuildInfo* Info = DataRow.GetRow<FBuildInfo>("PreConstruct_Load");
        if (Info && Info->Image)
        {
            AssetImage->SetBrushFromTexture(Info->Image);
        }
    }
}

void UBuildingSelectButtonWidget::OnBuildButtonClicked()
{
    CheckActive();

    if (!DataTable || !PlacementActorClass) return;

    const FBuildInfo* Info = DataRow.GetRow<FBuildInfo>("Click_Spawn");
    if (!Info || !Info->Mesh) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // PlacementActor ½ºÆù
    APlacementActor* NewActor = World->SpawnActor<APlacementActor>(
        PlacementActorClass,
        FVector::ZeroVector,
        FRotator::ZeroRotator
    );

    if (NewActor)
    {
        NewActor->SetMesh(Info->Mesh);
    }
}

void UBuildingSelectButtonWidget::CheckActive()
{
    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, APlacementActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (IsValid(Actor))
        {
            Actor->Destroy();
        }
    }
}