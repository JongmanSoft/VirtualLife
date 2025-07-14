// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSelectButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "PlacementActor.h"
#include "WallPlacementActor.h"

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

    if (Info)
    {
        UE_LOG(LogTemp, Log, TEXT("========== FBuildInfo 정보 출력 =========="));

        // Mesh 확인
        UE_LOG(LogTemp, Log, TEXT("Mesh: %s"), Info->Mesh ? *Info->Mesh->GetName() : TEXT("None"));

        // InteractableActorClass 확인
        UE_LOG(LogTemp, Log, TEXT("ActorClass: %s"), Info->InteractableActorClass ? *Info->InteractableActorClass->GetName() : TEXT("None"));

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Info is NULL. Row not found."));
    }

    if (!Info)
    {
        UE_LOG(LogTemp, Error, TEXT("[BuildingSelectButtonWidget] Row not found or Mesh missing for: %s"), *RowName.ToString());
        return;
    }

    if (!Info->Mesh && !Info->InteractableActorClass)
    {
        UE_LOG(LogTemp, Error, TEXT("[BuildingSelectButtonWidget] No Mesh or ActorClass assigned: %s"), *RowName.ToString());
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    APlacementActor* Spawned = nullptr;

    // UE_LOG(LogTemp, Warning, TEXT("bIsWall: % s"), Info->bIsWall ? TEXT("true") : TEXT("false"));
    // UE_LOG(LogTemp, Warning, TEXT("WallPlacementActorClass is valid: %s"), WallPlacementActorClass ? TEXT("true") : TEXT("false"));

    if (Info->bIsWall && WallPlacementActorClass)
    {
        AWallPlacementActor* Wall = World->SpawnActor<AWallPlacementActor>(WallPlacementActorClass);
        if (Wall)
        {
            // Wall->SetStartPoint(Wall->CachedMousePosition());
            Spawned = Wall;
            UE_LOG(LogTemp, Warning, TEXT("WallPlacementActorClass spawned"));
        }
    }
    else if (PlacementActorClass)
    {
        Spawned = World->SpawnActor<APlacementActor>(PlacementActorClass);
    }

    if (Spawned)
    {
        if (Info->Mesh)
        {
            UE_LOG(LogTemp, Error, TEXT("[BuildingSelectButtonWidget] Set Mesh"));
            Spawned->SetMesh(Info->Mesh);
        }
        else if (Info->InteractableActorClass)
        {
            Spawned->SetInteractableActorClass(Info->InteractableActorClass);
            UE_LOG(LogTemp, Log, TEXT("[BuildingSelectButtonWidget] Set InteractableActorClass to Spawned Actor"));
        }
        Spawned->SetPrice(Info->Price);
        Spawned->SetRowID(RowName);
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
