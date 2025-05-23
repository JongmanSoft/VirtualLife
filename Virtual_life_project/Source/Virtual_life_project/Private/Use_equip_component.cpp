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


#include "../interface/plant_able_cpp_interface.h"
#include "../interface/fishing_cpp_interface.h"
#include "../interface/mining_cpp_interface.h"

#include "Virtual_life_GameInstance.h"

#include "VL_Player.h"



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
    
    // SkeletalMesh 찾아서 OnMontageEnded 바인딩
    USkeletalMeshComponent* SkeletalMeshComp = ParentActor->FindComponentByClass<USkeletalMeshComponent>();
    if (SkeletalMeshComp)
    {
        UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
        if (AnimInstance && !AnimInstance->OnMontageEnded.IsAlreadyBound(this, &UUse_equip_component::OnMontageEnded))
        {
            AnimInstance->OnMontageEnded.AddDynamic(this, &UUse_equip_component::OnMontageEnded);
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
    PlayerInputComponent->BindAction("Key1", IE_Pressed, this, &UUse_equip_component::USE_None);
    PlayerInputComponent->BindAction("Key2", IE_Pressed, this, &UUse_equip_component::USE_None);
    PlayerInputComponent->BindAction("Key3", IE_Pressed, this, &UUse_equip_component::USE_None);
    PlayerInputComponent->BindAction("Key4", IE_Pressed, this, &UUse_equip_component::USE_None);
    PlayerInputComponent->BindAction("Key5", IE_Pressed, this, &UUse_equip_component::USE_None);
}

void UUse_equip_component::ChangeBindingFunc(uint8 index, uint8 tool_ID)
{

 

    static const TCHAR* key_string[5] = { TEXT("Key1"), TEXT("Key2"), TEXT("Key3"), TEXT("Key4"), TEXT("Key5") };

    // 기존 바인딩 제거
    InputComponent->RemoveActionBinding(key_string[index], IE_Pressed);

    // 새로운 바인딩 추가
    if (tool_ID == 0)
    {
        InputComponent->BindAction(key_string[index], IE_Pressed, this, ActionFunctions[tool_ID]);
    }
    else
    {
        InputComponent->BindAction(key_string[index], IE_Pressed, this, ActionFunctions[tool_ID - 7]);
    }

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

                    UGameplayStatics::PlaySound2D(this, LoadObject<USoundWave>(nullptr, TEXT("/Game/sound/effect_sound/water_dive.water_dive")));
                        UStaticMeshComponent* fishing_rod_mesh = Cast<UStaticMeshComponent>(ParentActor->FindComponentByTag(UStaticMeshComponent::StaticClass(), FName("FishingRod")));
                        if (fishing_rod_mesh)fishing_rod_mesh->SetVisibility(true);
                        if (SkeletalMeshComp)
                        {
                            UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
                            if (AnimInstance)
                            {
                                AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/metahuman_fishing_Montage.metahuman_fishing_Montage")), 1.0f);
                                State_update(FISH);
                            }

                            bool fishing_result = 0;
                            Ifishing_cpp_interface::Execute_fishing_func(Actor,fishing_result);
                            UVirtual_life_GameInstance* game_inst = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
                            game_inst->m_quest->finish_Quest(2); //낚시 퀘스트 조건만족
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
                State_update(SEED);
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
                        UVirtual_life_GameInstance* game_inst = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
                        game_inst->m_quest->finish_Quest(1); //농사 퀘스트 조건만족
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
                State_update(SEED);
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

                        UVirtual_life_GameInstance* game_inst = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
                        game_inst->m_quest->finish_Quest(1); //농사 퀘스트 조건만족

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
                State_update(MINE);
            }
        }

     
        UBoxComponent* boxComp = ParentActor->FindComponentByClass<UBoxComponent>();
        if (boxComp) {
            TArray<AActor*> OverlappingActors;
            boxComp->GetOverlappingActors(OverlappingActors);

            for (AActor* Actor : OverlappingActors)
            {
                if (Actor->IsA(Amining_spot::StaticClass()))
                {
                    Imining_cpp_interface::Execute_mining_func(Actor);
                    UVirtual_life_GameInstance* game_inst = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
                    game_inst->m_quest->finish_Quest(3); //채굴 퀘스트 조건만족
                }
            }
        }
    }
}

void UUse_equip_component::State_update(int st)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        APawn* ControlledPawn = PlayerController->GetPawn();
        AVL_Player* MyPlayer = Cast<AVL_Player>(ControlledPawn);
        if (MyPlayer)
        {
            MyPlayer->setMyState(st);
        }
    }
}

void UUse_equip_component::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    State_update(IDLE);
}
