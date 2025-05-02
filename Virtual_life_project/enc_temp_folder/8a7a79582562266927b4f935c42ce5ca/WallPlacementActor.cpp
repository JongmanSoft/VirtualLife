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

    if (!bHasStart)
    {
        StartPoint = MousePosition();
        SetActorLocation(StartPoint);
        return;
    }

    FVector EndPoint = MousePosition();
    UpdateWallPreview(EndPoint);
}

void AWallPlacementActor::SetStartPoint(const FVector& InStart)
{
    bHasStart = true;
    StartPoint = InStart;
}

void AWallPlacementActor::UpdateWallPreview(const FVector& Current)
{
    FVector Direction = Current - StartPoint;
    float Length = Direction.Size();

    FVector MidPoint = StartPoint + Direction * 0.5f;
    FRotator Rotation = Direction.Rotation();

    SetActorLocation(MidPoint);
    SetActorRotation(Rotation);
    SetActorScale3D(FVector(Length / 100.f, 1.0f, 1.0f));
}

void AWallPlacementActor::ConfirmWall()
{
    PlaceBuild();  // APlacementActor의 PlaceBuild 사용
}

void AWallPlacementActor::CancelWall()
{
    Destroy(); // 임시 벽 제거
}