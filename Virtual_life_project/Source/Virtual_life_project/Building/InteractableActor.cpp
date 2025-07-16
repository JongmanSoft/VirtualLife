// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Virtual_life_GameInstance.h"

AInteractableActor::AInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    Mesh->SetGenerateOverlapEvents(true);
    Mesh->SetNotifyRigidBodyCollision(true);
    Mesh->SetMobility(EComponentMobility::Movable);
    Mesh->SetSimulatePhysics(false);
}

void AInteractableActor::SendStateToServer(uint8 StateValue)
{
    if (auto* GI = Cast<UVirtual_life_GameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GI->set_state(StateValue);
    }
}

void AInteractableActor::SetMesh(UStaticMesh* StaticMesh)
{
    if (Mesh && StaticMesh)
    {
        Mesh->SetStaticMesh(StaticMesh);
    }
}