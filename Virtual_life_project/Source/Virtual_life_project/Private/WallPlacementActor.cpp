#include "WallPlacementActor.h"
#include "PlaceBuildActor.h"
#include "Kismet/GameplayStatics.h"

void AWallPlacementActor::BeginPlay()
{
    Super::BeginPlay();
}

void AWallPlacementActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    if (!bHasStart)
    {
        if (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton))
        {
            SetStartPoint(CachedMousePosition());
            UE_LOG(LogTemp, Warning, TEXT("Start Pos : % s"), *StartPoint.ToString());
        }
        else
        {
            SetActorLocation(CachedMousePosition());
        }
        return;
    }

    FVector EndPoint = CachedMousePosition();
    UpdateWallPreview(EndPoint);
}

void AWallPlacementActor::SetStartPoint(const FVector& InStart)
{
    bHasStart = true;
    StartPoint = InStart;
}

void AWallPlacementActor::UpdateWallPreview(const FVector& Current)
{
    FVector RawDirection = Current - StartPoint;

    // 거리 계산
    float Length = RawDirection.Size();
    if (Length < KINDA_SMALL_NUMBER)
        return;

    FVector Direction = FVector::ZeroVector;
    if (FMath::Abs(RawDirection.X) > FMath::Abs(RawDirection.Y))
    {
        Direction = FVector(FMath::Sign(RawDirection.X), 0.0f, 0.0f);
    }
    else
    {
        Direction = FVector(0.0f, FMath::Sign(RawDirection.Y), 0.0f);
    }

    float SnappedLength = FVector::DotProduct(RawDirection, Direction);

    FRotator Rotation = Direction.Rotation();
    SetActorLocation(StartPoint);
    SetActorRotation(Rotation);

    float ScaledLength = FMath::Abs(SnappedLength) / 100.f;
    SetActorScale3D(FVector(ScaledLength, 1.0f, 1.0f));
}

void AWallPlacementActor::ConfirmWall()
{
    PlaceBuild();
}

void AWallPlacementActor::CancelWall()
{
    Destroy();
}

FVector AWallPlacementActor::CachedMousePosition()
{
    FVector WorldLoc, WorldDir;
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return GetActorLocation();

    PC->DeprojectMousePositionToWorld(WorldLoc, WorldDir);
    FVector End = WorldLoc + WorldDir * 20000;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLoc, End, ECC_Visibility, Params))
    {
        return Hit.Location;
    }

    return GetActorLocation();
}
