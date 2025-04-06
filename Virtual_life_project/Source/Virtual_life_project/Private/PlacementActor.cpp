// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacementActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
//#include "PlaceBuildActor.h"

APlacementActor::APlacementActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
}

void APlacementActor::BeginPlay()
{
    Super::BeginPlay();
}

void APlacementActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector HitLoc = MousePosition();
    FVector Snapped = FVector(
        FMath::GridSnap(HitLoc.X, GridSize),
        FMath::GridSnap(HitLoc.Y, GridSize),
        FMath::GridSnap(HitLoc.Z, GridSize)
    );

    FRotator Rot(0.f, Rotate, 0.f);
    SetActorTransform(FTransform(Rot, Snapped, FVector(1.f)));
}

FVector APlacementActor::MousePosition()
{
    FVector WorldLoc, WorldDir;
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return GetActorLocation();

    PC->DeprojectMousePositionToWorld(WorldLoc, WorldDir);

    FVector End = WorldLoc + WorldDir * 20000;
    FHitResult Hit;

    GetWorld()->LineTraceSingleByChannel(Hit, WorldLoc, End, ECC_Visibility);
    return Hit.bBlockingHit ? Hit.Location : GetActorLocation();
}

void APlacementActor::SetMesh(UStaticMesh* NewMesh)
{
    if (NewMesh)
    {
        Mesh->SetStaticMesh(NewMesh);
    }
}

void APlacementActor::PlaceBuild()
{
    FVector Loc = GetActorLocation();
    FRotator Rot(0.f, Rotate, 0.f);
    //GetWorld()->SpawnActor<APlaceBuildActor>(APlaceBuildActor::StaticClass(), Loc, Rot);
    Destroy(); // 배치 완료 후 제거
}