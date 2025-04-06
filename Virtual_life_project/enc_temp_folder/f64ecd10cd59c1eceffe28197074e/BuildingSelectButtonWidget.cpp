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

    if (DataTable && AssetImage && !RowName.IsNone())
    {
        const FBuildInfo* Info = DataTable->FindRow<FBuildInfo>(RowName, TEXT("Preview"));
        if (Info && Info->Image)
        {
            AssetImage->SetBrushFromTexture(Info->Image);
        }
    }
}

void UBuildingSelectButtonWidget::OnBuildButtonClicked()
{
    CheckActive();

    if (!DataTable || RowName.IsNone()) return;

    const FBuildInfo* Info = DataTable->FindRow<FBuildInfo>(RowName, TEXT("Click"));
    if (!Info || !Info->Mesh) return;

    UWorld* World = GetWorld();
    if (!World) return;

    // PlacementActor_BP 블루프린트를 스폰
    APlacementActor* Placement = World->SpawnActor<APlacementActor>(
        APlacementActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

    if (Placement)
    {
        Placement->BuildingMesh = Info->Mesh;
    }
}

void UBuildingSelectButtonWidget::CheckActive()
{
    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(World, APlacementActor::StaticClass(), Found);

    for (AActor* Actor : Found)
    {
        if (IsValid(Actor))
        {
            Actor->Destroy();
        }
    }
}
