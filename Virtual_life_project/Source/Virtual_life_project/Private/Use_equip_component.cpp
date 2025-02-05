#include "Use_equip_component.h"
#include "GameFramework/Actor.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UUse_equip_component::UUse_equip_component()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UUse_equip_component::BeginPlay()
{
    Super::BeginPlay();

    AActor* ParentActor = GetOwner();
    if (ParentActor)
    {
        UInputComponent* InputComponent = ParentActor->FindComponentByClass<UInputComponent>();
        if (InputComponent)
        {
            SetupInputComponent(InputComponent);
        }
    }

  
}

// Called every frame
void UUse_equip_component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UUse_equip_component::SetupInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAction("Key1", IE_Pressed, this, &UUse_equip_component::USE_fishing_rod);
    PlayerInputComponent->BindAction("Key2", IE_Pressed, this, &UUse_equip_component::USE_tomato_seed);
    PlayerInputComponent->BindAction("Key3", IE_Pressed, this, &UUse_equip_component::USE_potato_seed);
    PlayerInputComponent->BindAction("Key4", IE_Pressed, this, &UUse_equip_component::USE_None);
    PlayerInputComponent->BindAction("Key5", IE_Pressed, this, &UUse_equip_component::USE_None);
}

void UUse_equip_component::USE_None()
{
}

void UUse_equip_component::USE_fishing_rod()
{
   
    AActor* ParentActor = GetOwner();
    if (ParentActor)
    {
        UStaticMeshComponent* fishing_rod_mesh = Cast<UStaticMeshComponent>(ParentActor->FindComponentByTag(UStaticMeshComponent::StaticClass(), FName("FishingRod")));
        if(fishing_rod_mesh)fishing_rod_mesh->SetVisibility(true);
        USkeletalMeshComponent* SkeletalMeshComp = ParentActor->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMeshComp)
        {
            UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
            if (AnimInstance )
            {
                AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/metahuman_fishing_Montage.metahuman_fishing_Montage")), 1.0f);
            }
        }
    }
  
}

void UUse_equip_component::USE_tomato_seed()
{
    AActor* ParentActor = GetOwner();
    if (ParentActor)
    {
       
        USkeletalMeshComponent* SkeletalMeshComp = ParentActor->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMeshComp)
        {
            UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/fast_plant.fast_plant")), 1.0f);
            }
        }

        UCapsuleComponent* capsuleComp = ParentActor->FindComponentByClass<UCapsuleComponent>();
        if (capsuleComp) {
          //  capsuleComp->GetOverlappingActors()

        }
    }
}

void UUse_equip_component::USE_potato_seed()
{
}
