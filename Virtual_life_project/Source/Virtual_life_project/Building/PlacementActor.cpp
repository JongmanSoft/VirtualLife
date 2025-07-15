// Fill out your copyright notice in the Description page of Project Settings.


#include "PlacementActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlaceBuildActor.h"
#include "WallPlacementActor.h"
#include "InteractableActor.h"
#include "Virtual_life_GameInstance.h"
#include "FloatingTextWidget.h"
#include "EngineUtils.h"
#include "BuildItemRegistry.h"
#include "FBuildInfo.h"
#include "BuildingPlayerController.h"



APlacementActor::APlacementActor()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}

void APlacementActor::BeginPlay()
{
    Super::BeginPlay();
}

void APlacementActor::SetRowID(FName InRowID)
{
    RowID = InRowID;
}

void APlacementActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector HitLoc = MousePosition();

    FVector Snapped = FVector(
        FMath::GridSnap(HitLoc.X, GridSize),
        FMath::GridSnap(HitLoc.Y, GridSize),
        HitLoc.Z
    );

    SetActorTransform(FTransform(FRotator(0.f, Rotate, 0.f), Snapped, FVector(CurrentScale)));

    bool bOutOfBounds = IsOutOfBounds(Snapped);
    bool bOverlapping = IsOverlapping();
    bool bCanPlace = !(bOutOfBounds || bOverlapping);

    UpdateOverlayColor(bCanPlace);
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

        if (OverlayMaterial)
        {
            DynMaterial = UMaterialInstanceDynamic::Create(OverlayMaterial, this);
            Mesh->SetMaterial(0, DynMaterial);
            Mesh->SetRenderCustomDepth(true);
            Mesh->SetCustomDepthStencilValue(1);

            // 초기 색상 설정
            UpdateOverlayColor(true);
        }
    }
}

void APlacementActor::PlaceBuild()
{
    UE_LOG(LogTemp, Warning, TEXT("==== PlaceBuild() Start ===="));

    if (auto* MyGI = Cast<UVirtual_life_GameInstance>(GetGameInstance()))
    {
        if (MyGI->GetCurrentGold() < BuildPrice)
        {
            UE_LOG(LogTemp, Warning, TEXT("Not enough gold: %d / %d"), MyGI->GetCurrentGold(), BuildPrice);
            return;
        }

        if (PlaceSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, PlaceSound, GetActorLocation());
        }

        MyGI->SendUpdateGoldPacket(-BuildPrice);
        MyGI->ShowFloatingText(FString::Printf(TEXT("-%d"), BuildPrice), FLinearColor::Yellow, GetActorLocation());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance cast failed"));
        return;
    }

    // 위치 보정 포함한 최종 Transform
    FVector FinalLocation = GetActorLocation();
    FVector FinalScale = GetActorScale3D();
    FRotator FinalRotation = GetActorRotation();
    FTransform FinalTransform(FinalRotation, FinalLocation, FinalScale);

    if (Mesh && Mesh->GetStaticMesh())
    {
        FVector Origin, Extent;
        Mesh->GetLocalBounds(Origin, Extent);
        FinalLocation.Z -= Origin.Z;
        FinalTransform.SetLocation(FinalLocation);
    }

    AActor* SpawnedActor = nullptr;

    // InteractableActorClass 우선
    if (InteractableActorClass)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        auto* Interactable = GetWorld()->SpawnActor<AInteractableActor>(InteractableActorClass, FinalTransform, Params);
        SpawnedActor = Interactable;

        if (Interactable)
        {
            Interactable->SetRowID(RowID);
            Interactable->SetActorScale3D(FinalScale);

            if (Mesh && Mesh->GetStaticMesh())
            {
                if (auto* MeshComp = Interactable->FindComponentByClass<UStaticMeshComponent>())
                {
                    MeshComp->SetStaticMesh(Mesh->GetStaticMesh());
                }
            }
        }

        // DrawDebugBox(GetWorld(), FinalTransform.GetLocation(), FVector(30), FColor::Red, false, 5.f);
    }
    // 일반 PlaceBuildActor
    else if (PlaceBuildClass && Mesh)
    {
        auto* Placed = GetWorld()->SpawnActor<APlaceBuildActor>(PlaceBuildClass, FinalTransform);
        if (Placed)
        {
            Placed->SetMesh(Mesh->GetStaticMesh());
            Placed->SetScale(FinalScale.X, IsA(AWallPlacementActor::StaticClass()));
            Placed->SetRowID(RowID);
            SpawnedActor = Placed;
        }
    }

    // 서버 동기화 및 프리뷰 제거
    if (SpawnedActor)
    {
        if (auto* PC = Cast<ABuildingPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
        {
            FObjectData NewData;
            NewData.ItemID = FBuildItemRegistry::FNameToItemID(RowID);
            NewData.Location = FinalLocation;
            NewData.Yaw = FinalRotation.Yaw;
            NewData.Scale = FinalScale.X;

            PC->AddPendingBuild(NewData);
        }

        Destroy(); // 프리뷰 제거
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnedActor is NULL"));
    }

    UE_LOG(LogTemp, Warning, TEXT("==== PlaceBuild() End ===="));
}


void APlacementActor::AddRotation(float Delta)
{
    Rotate = FMath::Fmod(Rotate + Delta, 360.f);
}

void APlacementActor::SetPrice(int32 Price)
{
    BuildPrice = Price;
}

void APlacementActor::AdjustScaleByWheel(float AxisValue)
{
    CurrentScale += AxisValue * ScaleStep;
    CurrentScale = FMath::Clamp(CurrentScale, MinScale, MaxScale);
}


void APlacementActor::UpdateOverlayColor(bool bCanPlace)
{
    if (DynMaterial)
    {
        FLinearColor Color = bCanPlace ? FLinearColor(0.0f, 1.0f, 0.0f) : FLinearColor(1.0f, 0.0f, 0.0f);
        DynMaterial->SetVectorParameterValue(TEXT("OverlayColor"), Color);
        DynMaterial->SetScalarParameterValue(TEXT("Opacity"), 0.3f);
    }
}

bool APlacementActor::IsOutOfBounds(const FVector& Location) const
{
    const FVector2D Min(-3610.f, -10280.f);
    const FVector2D Max(-1210.f, -7850.f);

    return !(Location.X >= Min.X && Location.X <= Max.X &&
        Location.Y >= Min.Y && Location.Y <= Max.Y);
}

bool APlacementActor::IsOverlapping() const
{
    for (TActorIterator<APlaceBuildActor> It(GetWorld()); It; ++It)
    {
        const APlaceBuildActor* Other = *It;
        if (!Other || Other == Cast<APlaceBuildActor>(this)) continue;

        const FBox MyBox = Mesh->Bounds.GetBox();
        const FBox OtherBox = Other->GetComponentsBoundingBox();

        if (MyBox.Intersect(OtherBox))
        {
            return true;
        }
    }
    return false;
}