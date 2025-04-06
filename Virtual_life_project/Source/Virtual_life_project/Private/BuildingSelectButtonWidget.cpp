// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSelectButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "PlacementActor.h"

void UBuildingSelectButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (DataTable)
    {
        FBuildInfo* RowData = DataTable->FindRow<FBuildInfo>(RowName, "");
        if (RowData && AssetImage)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(RowData->Image);
            AssetImage->SetBrush(Brush);
        }
    }

    if (Build_BTN)
    {
        Build_BTN->OnClicked.AddDynamic(this, &UBuildingSelectButtonWidget::OnClickedBuildButton);
    }
}

void UBuildingSelectButtonWidget::OnClickedBuildButton()
{
    CheckActive();

    if (!DataTable) return;
    FBuildInfo* Info = DataTable->FindRow<FBuildInfo>(RowName, "");
    if (!Info) return;

    UWorld* World = GetWorld();
    if (World)
    {
        APlacementActor* Spawned = World->SpawnActor<APlacementActor>(APlacementActor::StaticClass());
        if (Spawned)
        {
            Spawned->SetMesh(Info->Mesh);
        }
    }
}

void UBuildingSelectButtonWidget::CheckActive_Implementation()
{
    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, APlacementActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (Actor && Actor->IsValidLowLevel())
        {
            Actor->Destroy();
        }
    }
}
