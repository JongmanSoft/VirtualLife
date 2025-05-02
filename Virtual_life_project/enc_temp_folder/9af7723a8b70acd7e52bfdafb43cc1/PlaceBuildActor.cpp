// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceBuildActor.h"
#include "Components/StaticMeshComponent.h"
#include "BuildingPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values
APlaceBuildActor::APlaceBuildActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

    // 클릭 이벤트 활성화
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    Mesh->SetGenerateOverlapEvents(true);
    Mesh->SetNotifyRigidBodyCollision(true);
    Mesh->SetMobility(EComponentMobility::Movable);
    Mesh->SetSimulatePhysics(false);
}

void APlaceBuildActor::SetMesh(UStaticMesh* StaticMesh)
{
    if (StaticMesh)
    {
        Mesh->SetStaticMesh(StaticMesh);
    }
}

void APlaceBuildActor::SetScale(float InScale)
{
    SetActorScale3D(FVector(InScale));
}
