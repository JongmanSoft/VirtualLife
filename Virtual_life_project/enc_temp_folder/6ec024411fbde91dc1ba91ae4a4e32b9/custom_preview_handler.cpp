// Fill out your copyright notice in the Description page of Project Settings.

#include "custom_preview_handler.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"

// Sets default values for this component's properties
Ucustom_preview_handler::Ucustom_preview_handler()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void Ucustom_preview_handler::BeginPlay()
{
    Super::BeginPlay();

    m_owner = GetOwner();
    if (!m_owner)
    {
        return;
    }

    // 입력 바인딩 설정 호출
    SetupInputComponent();
}

// 입력 바인딩 설정 (PlayerController에서 처리)
void Ucustom_preview_handler::SetupInputComponent()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        UInputComponent* InputComp = PC->InputComponent;
        if (InputComp)
        {
            InputComp->BindAction("LeftMouseButton", IE_Pressed, this, &Ucustom_preview_handler::OnLeftMouseClick);
            InputComp->Priority = 10;
          
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Ucustom_preview_handler: PlayerController has no InputComponent!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Ucustom_preview_handler: No PlayerController found!"));
    }
}

// 마우스 클릭 처리
void Ucustom_preview_handler::OnLeftMouseClick()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    FVector WorldLocation, WorldDirection;
    PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

    FVector TraceStart = WorldLocation;
    FVector TraceEnd = TraceStart + (WorldDirection * 10000.f);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(m_owner);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_GameTraceChannel1,
        QueryParams
    );

    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2.f);

    if (bHit)
    {
        UPrimitiveComponent* HitComponent = HitResult.GetComponent();
        if (HitComponent)
        {
            UE_LOG(LogTemp, Log, TEXT("Clicked a collision on ECC_GameTraceChannel1! Actor: %s, Component: %s"),
                *HitResult.GetActor()->GetName(), *HitComponent->GetName());
            DrawDebugPoint(GetWorld(), HitResult.Location, 10.f, FColor::Green, false, 2.f);
        }
    }
}