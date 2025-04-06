// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacementActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

APlacementActor::APlacementActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
}

void APlacementActor::BeginPlay()
{
    Super::BeginPlay();

    if (BuildingMesh)
    {
        Mesh->SetStaticMesh(BuildingMesh);
        UMaterialInterface* Glow = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/BuildingSystem/Glow_M.Glow_M"));
        if (Glow)
        {
            Mesh->SetOverlayMaterial(Glow);
        }
    }
}

void APlacementActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetActorLocation(GetMouseSnappedPosition());
}

FVector APlacementActor::GetMouseSnappedPosition() const
{
    FVector WorldLocation, WorldDirection;

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
        {
            FVector Start = WorldLocation;
            FVector End = Start + WorldDirection * 10000.0f;

            FHitResult Hit;
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(this);

            if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
            {
                FVector P = Hit.Location;
                return FVector(
                    FMath::GridSnap(P.X, GridSize),
                    FMath::GridSnap(P.Y, GridSize),
                    FMath::GridSnap(P.Z, GridSize)
                );
            }
        }
    }

    return GetActorLocation();
}
