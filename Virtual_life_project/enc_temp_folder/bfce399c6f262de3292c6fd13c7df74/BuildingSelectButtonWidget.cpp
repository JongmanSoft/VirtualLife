// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSelectButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "PlacementActor.h"

void UBuildingSelectButtonWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (DataTable && RowName != NAME_None)
    {
        FBuildInfo* RowData = DataTable->FindRow<FBuildInfo>(RowName, "");
        if (RowData)
        {
            if (AssetImage)
            {
                FSlateBrush Brush;
                Brush.SetResourceObject(RowData->Image);
                AssetImage->SetBrush(Brush);
            }

            if (NameText)
            {
                NameText->SetText(RowData->Name);
            }

            if (DescriptionText)
            {
                DescriptionText->SetText(RowData->Description);
            }

            if (PriceText)
            {
                PriceText->SetText(FText::AsNumber(RowData->Price));
            }
        }
    }
}

void UBuildingSelectButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Build_BTN && !Build_BTN->OnClicked.IsAlreadyBound(this, &UBuildingSelectButtonWidget::OnClickedBuildButton))
    {
        Build_BTN->OnClicked.AddDynamic(this, &UBuildingSelectButtonWidget::OnClickedBuildButton);
    }
}

void UBuildingSelectButtonWidget::OnClickedBuildButton()
{
    if (!DataTable || RowName == NAME_None)
    {
        UE_LOG(LogTemp, Error, TEXT("[BuildingSelectButtonWidget] DataTable or RowName is invalid."));
        return;
    }

    CheckActive();

    FBuildInfo* Info = DataTable->FindRow<FBuildInfo>(RowName, "");
    if (!Info || !Info->Mesh)
    {
        UE_LOG(LogTemp, Error, TEXT("[BuildingSelectButtonWidget] Row not found or Mesh missing for: %s"), *RowName.ToString());
        return;
    }

    UWorld* World = GetWorld();

    if (World && PlacementActorClass)
    {
        APlacementActor* Spawned = World->SpawnActor<APlacementActor>(PlacementActorClass);
        if (Spawned)
        {
            Spawned->SetMesh(Info->Mesh);
            Spawned->SetPrice(Info->Price);
            Spawned->SetRowID(RowName);
            UE_LOG(LogTemp, Warning, TEXT("Spawned: %s"), *GetNameSafe(Spawned));
        }
    }
}

void UBuildingSelectButtonWidget::SetRowData(UDataTable* InDataTable, FName InRowName)
{
    DataTable = InDataTable;
    RowName = InRowName;

    // 즉시 이미지 업데이트
    NativePreConstruct();
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
