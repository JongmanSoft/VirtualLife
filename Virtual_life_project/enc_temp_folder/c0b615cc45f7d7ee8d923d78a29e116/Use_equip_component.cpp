#include "Use_equip_component.h"
#include "GameFramework/Actor.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "seed_spot.h"
#include "fishing_spot.h"
#include "mining_spot.h"

#include "Kismet/GameplayStatics.h"


#include "plant_able_cpp_interface.h"
#include "fishing_cpp_interface.h"
#include "mining_cpp_interface.h"

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
    PlayerInputComponent->BindAction("Key4", IE_Pressed, this, &UUse_equip_component::USE_pickaxe);
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
        //플레이어 메쉬찾기
        USkeletalMeshComponent* SkeletalMeshComp = ParentActor->FindComponentByClass<USkeletalMeshComponent>();
        //우선 피싱스폿 안에 들어왔는지 확인 
        UCapsuleComponent* capsuleComp = ParentActor->FindComponentByClass<UCapsuleComponent>();
        if (capsuleComp) {
            TArray<AActor*> OverlappingActors;
            capsuleComp->GetOverlappingActors(OverlappingActors);

            for (AActor* Actor : OverlappingActors)
            {
                if (Actor->IsA(Afishing_spot::StaticClass())) {

                    UGameplayStatics::PlaySound2D(this, LoadObject<USoundWave>(nullptr, TEXT("/Game/sound/effect_sound/water.water")));
                        UStaticMeshComponent* fishing_rod_mesh = Cast<UStaticMeshComponent>(ParentActor->FindComponentByTag(UStaticMeshComponent::StaticClass(), FName("FishingRod")));
                        if (fishing_rod_mesh)fishing_rod_mesh->SetVisibility(true);
                        if (SkeletalMeshComp)
                        {
                            UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
                            if (AnimInstance)
                            {
                                AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/metahuman_fishing_Montage.metahuman_fishing_Montage")), 1.0f);
                            }

                            bool fishing_result = 0;
                            Ifishing_cpp_interface::Execute_fishing_func(Actor,fishing_result);

                        }
                    


                }
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
            TArray<AActor*> OverlappingActors;
            capsuleComp->GetOverlappingActors(OverlappingActors);

            for (AActor* Actor : OverlappingActors)
            {
                if (Actor->IsA(Aseed_spot::StaticClass()))
                {

                    bool plant_result = 0;
                    Iplant_able_cpp_interface::Execute_plant_what_func(Actor, 0, plant_result);

                    if (plant_result) {
                        UGameplayStatics::PlaySound2D(this, LoadObject<USoundWave>(nullptr, TEXT("/Game/sound/effect_sound/Use_seeds.Use_seeds")));
                        
                    }
                    
                }
            }

        }
    }

}

void UUse_equip_component::USE_potato_seed()
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
            TArray<AActor*> OverlappingActors;
            capsuleComp->GetOverlappingActors(OverlappingActors);

            for (AActor* Actor : OverlappingActors)
            {
                if (Actor->IsA(Aseed_spot::StaticClass()))
                {

                    bool plant_result = 0;
                    Iplant_able_cpp_interface::Execute_plant_what_func(Actor, 1, plant_result);

                    if (plant_result) {
                        UGameplayStatics::PlaySound2D(this, LoadObject<USoundWave>(nullptr, TEXT("/Game/sound/effect_sound/Use_seeds.Use_seeds")));

                    }

                }
            }

        }
    }
}

void UUse_equip_component::USE_pickaxe()
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
                UStaticMeshComponent* pickaxe_mesh = Cast<UStaticMeshComponent>(ParentActor->FindComponentByTag(UStaticMeshComponent::StaticClass(), FName("Pickaxe")));
                if (pickaxe_mesh)pickaxe_mesh->SetVisibility(true);
                AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/slash_pickaxe_montage.slash_pickaxe_montage")), 1.0f);
            }
        }

        UCapsuleComponent* capsuleComp = ParentActor->FindComponentByClass<UCapsuleComponent>();
        if (capsuleComp) {
            TArray<AActor*> OverlappingActors;
            capsuleComp->GetOverlappingActors(OverlappingActors);

            for (AActor* Actor : OverlappingActors)
            {
                if (Actor->IsA(Amining_spot::StaticClass()))
                {
                    bool plant_result = 0;
                    Imining_cpp_interface::Execute_mining_func(Actor);

                }
            }

        }
    }
}
