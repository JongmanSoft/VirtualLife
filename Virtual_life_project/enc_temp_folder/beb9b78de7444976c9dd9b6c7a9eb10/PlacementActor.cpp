// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacementActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlaceBuildActor.h"
#include "Virtual_life_GameInstance.h"
#include "FloatingTextWidget.h"

APlacementActor::APlacementActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // 자기 자신 무시
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
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
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    GetWorld()->LineTraceSingleByChannel(Hit, WorldLoc, End, ECC_Visibility, Params);
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
    UGameInstance* GI = GetGameInstance();
    if (!GI) return;

    auto MyGI = Cast<UVirtual_life_GameInstance>(GI);
    if (!MyGI) return;

    if (!MyGI || MyGI->GetCurrentGold() < BuildPrice)
    {
        // 돈 부족 음 !
        return;
    }

    // 골드 차감
    MyGI->SendUpdateGoldPacket(-BuildPrice);

    // UI 알림
    FString Text = FString::Printf(TEXT("-%d"), BuildPrice);
    MyGI->ShowFloatingText(Text, FLinearColor::Red, GetActorLocation());

    // 실제 배치
    FVector Loc = GetActorLocation();
    FRotator Rot(0.f, Rotate, 0.f);

    FVector AdjustedLoc = Loc;
    if (Mesh && Mesh->GetStaticMesh())
    {
        FVector Origin, BoxExtent;
        Mesh->GetLocalBounds(Origin, BoxExtent);
        AdjustedLoc.Z -= Origin.Z;
    }

    APlaceBuildActor* Placed = GetWorld()->SpawnActor<APlaceBuildActor>(PlaceBuildClass, Loc, Rot);
    if (Placed && Mesh)
    {
        Placed->SetMesh(Mesh->GetStaticMesh());
    }

    Destroy(); // 제거
}

void APlacementActor::AddRotation(float Delta)
{
    Rotate = FMath::Fmod(Rotate + Delta, 360.f);
}

void APlacementActor::SetPrice(int32 Price)
{
    BuildPrice = Price;
}
