// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingInteractionWidget.h"
#include "Components/Button.h"
#include "BuildingPlayerController.h"
#include "PlacementActor.h"
#include "PlaceBuildActor.h"
#include "Kismet/GameplayStatics.h"
#include "Virtual_life_GameInstance.h"


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
    UStaticMesh* Mesh = PC->SelectedBuildActor->Mesh->GetStaticMesh();
    FName RowID = PC->SelectedBuildActor->GetRowID();

    if (auto GI = Cast<UVirtual_life_GameInstance>(GetGameInstance()))
    {
        GI->SendRemoveBuildPacket(Location);
    }

    PC->RemovePendingBuildAtLocation(Location);

    PC->SelectedBuildActor->Destroy();

    APlacementActor* NewPreview = GetWorld()->SpawnActor<APlacementActor>(
        PC->PlacementActorClass, Location, Rotation);

    if (NewPreview && Mesh)
    {
        NewPreview->SetMesh(Mesh);
        NewPreview->SetRowID(RowID);
        NewPreview->SetPrice(0);
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