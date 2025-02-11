#include "Use_equip_component.h"
#include "GameFramework/Actor.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"


#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

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
        InputComponent = ParentActor->FindComponentByClass<UInputComponent>();
        if (InputComponent)
        {
            SetupInputComponent(InputComponent);
        }
    }

    ActionFunctions[0] = &UUse_equip_component::USE_None;
    ActionFunctions[1] = &UUse_equip_component::USE_potato_seed;
    ActionFunctions[2] = &UUse_equip_component::USE_tomato_seed;
    ActionFunctions[3] = &UUse_equip_component::USE_fishing_rod;
    ActionFunctions[4] = &UUse_equip_component::USE_pickaxe;
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

void UUse_equip_component::ChangeBindingFunc(uint8 index, uint8 tool_ID)
{
    static const char key_string[5][5] = {"Key1","key2","key3" ,"key4" ,"key5" };
 
    if(tool_ID == 0)  InputComponent->BindAction(key_string[index], IE_Pressed, this, ActionFunctions[tool_ID]);
    else InputComponent->BindAction(key_string[index], IE_Pressed, this, ActionFunctions[tool_ID-7]);

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

        //아래의 캡슐컴포넌트와 오버랩된 객체를 검사하는 부분을 캡슐컴포넌트가 아닌 box collision으로 바꾸고싶음,
        UBoxComponent* boxComp = ParentActor->FindComponentByClass<UBoxComponent>();
        if (boxComp) {
            TArray<AActor*> OverlappingActors;
            boxComp->GetOverlappingActors(OverlappingActors);

            for (AActor* Actor : OverlappingActors)
            {
                if (Actor->IsA(Amining_spot::StaticClass()))
                {
                    Imining_cpp_interface::Execute_mining_func(Actor);
                }
            }
        }
    }
}
