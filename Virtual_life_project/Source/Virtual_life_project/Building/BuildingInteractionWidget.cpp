// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingInteractionWidget.h"
#include "Components/Button.h"
#include "BuildingPlayerController.h"
#include "PlacementActor.h"
#include "PlaceBuildActor.h"
#include "InteractableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Virtual_life_GameInstance.h"
#include "BuildItemRegistry.h"


void UBuildingInteractionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Modify_BTN)
        Modify_BTN->OnClicked.AddDynamic(this, &UBuildingInteractionWidget::OnClickModify);

    if (Delete_BTN)
        Delete_BTN->OnClicked.AddDynamic(this, &UBuildingInteractionWidget::OnClickDelete);
}

void UBuildingInteractionWidget::OnClickModify()
{
    auto PC = Cast<ABuildingPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PC || !PC->SelectedBuildActor || !PC->PlacementActorClass) return;

    FVector Location = PC->SelectedBuildActor->GetActorLocation();
    FRotator Rotation = PC->SelectedBuildActor->GetActorRotation();

    FName RowID;

    if (APlaceBuildActor* PlaceActor = Cast<APlaceBuildActor>(PC->SelectedBuildActor))
    {
        RowID = PlaceActor->GetRowID();
    }
    else if (AInteractableActor* InteractableActor = Cast<AInteractableActor>(PC->SelectedBuildActor))
    {
        RowID = InteractableActor->RowID;
    }
    else
    {
        return;
    }

    const FBuildInfo* Info = FBuildItemRegistry::FindBuildInfo(RowID);
    if (!Info) return;

    if (auto GI = Cast<UVirtual_life_GameInstance>(GetGameInstance()))
    {
        GI->SendRemoveBuildPacket(Location);
    }

    PC->RemovePendingBuildAtLocation(Location);
    PC->SelectedBuildActor->Destroy();

    APlacementActor* NewPreview = GetWorld()->SpawnActor<APlacementActor>(PC->PlacementActorClass, Location, Rotation);
    if (NewPreview)
    {
        if (Info->Mesh)
        {
            NewPreview->SetMesh(Info->Mesh);
        }

        NewPreview->SetRowID(RowID);
        NewPreview->SetPrice(0); // 수정이므로 가격 0으로
    }

    RemoveFromParent();
}

void UBuildingInteractionWidget::OnClickDelete()
{
    auto PC = Cast<ABuildingPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (!PC || !PC->SelectedBuildActor) return;

    FVector Location = PC->SelectedBuildActor->GetActorLocation();

    if (auto GI = Cast<UVirtual_life_GameInstance>(GetGameInstance()))
    {
        GI->SendRemoveBuildPacket(Location); // 서버로 전송!!
    }

    PC->RemovePendingBuildAtLocation(Location);
    PC->SelectedBuildActor->Destroy();
    PC->SelectedBuildActor = nullptr;

    RemoveFromParent(); // UI 제거
}