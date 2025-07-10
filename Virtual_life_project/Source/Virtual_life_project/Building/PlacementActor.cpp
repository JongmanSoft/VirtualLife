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

void APlacementActor::SetInteractableActorClass(TSubclassOf<AActor> InClass)
{
    InteractableActorClass = InClass;

    if (InteractableActorClass)
    {
        AActor* DefaultActor = InteractableActorClass->GetDefaultObject<AActor>();
        if (DefaultActor)
        {
            if (UStaticMeshComponent* MeshComp = DefaultActor->FindComponentByClass<UStaticMeshComponent>())
            {
                SetMesh(MeshComp->GetStaticMesh());
            }
        }
    }

    if (OverlayMaterial && Mesh)
    {
        DynMaterial = UMaterialInstanceDynamic::Create(OverlayMaterial, this);
        Mesh->SetMaterial(0, DynMaterial);
        Mesh->SetRenderCustomDepth(true);
        Mesh->SetCustomDepthStencilValue(1);
        UpdateOverlayColor(true);
    }
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
    UGameInstance* GI = GetGameInstance();
    if (!GI) return;

    auto MyGI = Cast<UVirtual_life_GameInstance>(GI);
    if (!MyGI) return;

    if (!MyGI || MyGI->GetCurrentGold() < BuildPrice)
    {
        // 돈 부족 음 !
        return;
    }

    if (PlaceSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, PlaceSound, GetActorLocation());
    }

    // 골드 차감
    MyGI->SendUpdateGoldPacket(-BuildPrice);

    // UI 알림
    FString Text = FString::Printf(TEXT("-%d"), BuildPrice);
    MyGI->ShowFloatingText(Text, FLinearColor::Yellow, GetActorLocation());

    FTransform FinalTransform;
    FVector FinalLocation;
    float FinalYaw = 0.f;
    float FinalScale = CurrentScale;

    if (IsA(AWallPlacementActor::StaticClass()))
    {
        FVector PreviewScale = GetActorScale3D();
        float FinalLength = PreviewScale.X;

        FinalTransform = FTransform(GetActorRotation(), GetActorLocation(), FVector(FinalLength, 1.0f, 1.0f));
        FinalLocation = GetActorLocation();
        FinalYaw = GetActorRotation().Yaw;
        FinalScale = GetActorScale3D().X;
    }
    else
    {
        FVector Loc = GetActorLocation();
        FRotator Rot(0.f, Rotate, 0.f);

        FVector AdjustedLoc = Loc;
        if (Mesh && Mesh->GetStaticMesh())
        {
            FVector Origin, BoxExtent;
            Mesh->GetLocalBounds(Origin, BoxExtent);
            AdjustedLoc.Z -= Origin.Z;
        }

        FinalTransform = FTransform(Rot, AdjustedLoc, FVector(CurrentScale));
        FinalLocation = AdjustedLoc;
        FinalYaw = Rot.Yaw;
        FinalScale = CurrentScale;
    }

    AActor* SpawnedActor = nullptr;

    if (InteractableActorClass) // ---------------------------------------------------------------
    {
        SpawnedActor = GetWorld()->SpawnActor<AActor>(InteractableActorClass, FinalTransform);

        if (AInteractableActor* Interactable = Cast<AInteractableActor>(SpawnedActor))
        {
            Interactable->SetRowID(RowID);

            Interactable->SetActorScale3D(FVector(FinalScale));

            if (UStaticMeshComponent* MeshComp = Interactable->FindComponentByClass<UStaticMeshComponent>())
            {
                if (Mesh && Mesh->GetStaticMesh())
                {
                    MeshComp->SetStaticMesh(Mesh->GetStaticMesh());
                }
            }
        }
    }
    else if (PlaceBuildClass && Mesh)
    {
        APlaceBuildActor* Placed = GetWorld()->SpawnActor<APlaceBuildActor>(PlaceBuildClass, FinalTransform);
        if (Placed)
        {
            Placed->SetMesh(Mesh->GetStaticMesh());
            bool bZOnly = IsA(AWallPlacementActor::StaticClass());
            Placed->SetScale(FinalScale, bZOnly);
            Placed->SetRowID(RowID);
            SpawnedActor = Placed;
        }
    }

    if (SpawnedActor)
    {
        auto PC = Cast<ABuildingPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        if (PC)
        {
            uint16 ItemID = FBuildItemRegistry::FNameToItemID(RowID);

            FObjectData NewData;
            NewData.ItemID = ItemID;
            NewData.Location = FinalLocation;
            NewData.Yaw = FinalYaw;
            NewData.Scale = FinalScale;

            PC->AddPendingBuild(NewData);
        }

        Destroy();
    }
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