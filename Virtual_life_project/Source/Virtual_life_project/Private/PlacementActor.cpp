// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacementActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
APlacementActor::APlacementActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void APlacementActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (BuildingMesh)
	{
		MeshComponent->SetStaticMesh(BuildingMesh);
	}
    UMaterialInterface* GlowMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/BuildingSystem/Glow_M.Glow_M"));
    if (GlowMaterial)
    {
        MeshComponent->SetOverlayMaterial(GlowMaterial);
    }
}

// Called every frame
void APlacementActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CachedLocation = GetMouseSnappedPosition();
	SetActorLocation(CachedLocation);

	FRotator Rot = FRotator(0.0f, RotationAngle, 0.0f);
	SetActorRotation(Rot);
}

FVector APlacementActor::GetMouseSnappedPosition() const
{
    FVector WorldLocation, WorldDirection;

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
        {
            FVector TraceStart = WorldLocation;
            FVector TraceEnd = TraceStart + (WorldDirection * 20000.0f);

            FHitResult Hit;
            FCollisionQueryParams Params;
            Params.bTraceComplex = true;
            Params.AddIgnoredActor(this);

            if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
            {
                FVector HitLocation = Hit.Location;
                return FVector(
                    FMath::GridSnap(HitLocation.X, GridSize),
                    FMath::GridSnap(HitLocation.Y, GridSize),
                    FMath::GridSnap(HitLocation.Z, GridSize)
                );
            }
        }
    }

    return GetActorLocation();
}

void APlacementActor::SetMesh(UStaticMesh* InMesh)
{
    BuildingMesh = InMesh;

    if (MeshComponent && InMesh)
    {
        MeshComponent->SetStaticMesh(InMesh);
    }
}

void APlacementActor::PlaceBuild()
{
    FActorSpawnParameters Params;
    Params.Owner = this;

    FVector SpawnLocation = CachedLocation;
    FRotator SpawnRotation = FRotator(0.f, RotationAngle, 0.f);

    APlacementActor* Placed = GetWorld()->SpawnActor<APlacementActor>(APlacementActor::StaticClass(), SpawnLocation, SpawnRotation, Params);
    if (Placed)
    {
        Placed->BuildingMesh = this->BuildingMesh;
        Placed->GridSize = this->GridSize;
        Placed->RotationAngle = this->RotationAngle;
        Placed->MeshComponent->SetStaticMesh(BuildingMesh);
    }

    ////////////////////////////////////////// ¼­¹ö
    
}

