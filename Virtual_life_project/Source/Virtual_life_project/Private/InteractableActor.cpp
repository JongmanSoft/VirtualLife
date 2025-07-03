// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/Engine.h"

AInteractableActor::AInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    CollisionBox->SetGenerateOverlapEvents(true);
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnOverlapEnd);

    bIsInteracting = false;
    InteractingActor = nullptr;
}

void AInteractableActor::BeginPlay()
{
    Super::BeginPlay();
}

void AInteractableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->GetClass()->ImplementsInterface(UCharacter_interface::StaticClass()))
    {
        if (ICharacter_interface::Execute_IsPlayer(OtherActor))
        {
            InteractingActor = OtherActor;

            if (InteractWidgetClass)
            {
                W_Interact = CreateWidget(GetWorld(), InteractWidgetClass);
                if (W_Interact)
                {
                    W_Interact->AddToViewport();
                }
            }

            if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
            {
                EnableInput(PC);

                if (InputComponent)
                {
                    InputComponent->BindAction("Interact", IE_Pressed, this, &AInteractableActor::HandleInteract);
                }
            }
        }
    }
}

void AInteractableActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor == InteractingActor)
    {
        if (OtherActor->GetClass()->ImplementsInterface(UCharacter_interface::StaticClass()))
        {
            if (ICharacter_interface::Execute_IsPlayer(OtherActor))
            {
                if (W_Interact)
                {
                    W_Interact->RemoveFromParent();
                    W_Interact = nullptr;
                }

                if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
                {
                    DisableInput(PC);
                }

                InteractingActor = nullptr;
            }
        }
    }
}

void AInteractableActor::HandleInteract()
{
    if (!InteractingActor) return;

    if (!bIsInteracting)
    {
        StartInteraction();
    }
    else
    {
        EndInteraction();
    }
}

void AInteractableActor::StartInteraction()
{
    bIsInteracting = true;

    if (W_Interact)
    {
        W_Interact->RemoveFromParent();
        W_Interact = nullptr;
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Player Interacting"));

    // 이후 자식 클래스에서 오버라이드
}

void AInteractableActor::EndInteraction()
{
    bIsInteracting = false;

    // 이후 자식 클래스에서 오버라이드
}