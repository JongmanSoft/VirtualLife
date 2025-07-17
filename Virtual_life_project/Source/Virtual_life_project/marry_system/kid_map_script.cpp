// Fill out your copyright notice in the Description page of Project Settings.


#include "kid_map_script.h"
#include "Blueprint/UserWidget.h"
#include "../Public/Virtual_life_GameInstance.h"
#include "../Custom/m_CustomizableSkeletalComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

void Akid_map_script::BeginPlay()
{
    Super::BeginPlay(); // 부모 클래스의 BeginPlay 호출
    // 플레이어 컨트롤러 가져오기
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // 위젯 클래스 로드
        TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/MyKid_custom_ui.MyKid_custom_ui_C"));
        if (WidgetClass)
        {
            WidgetInstance = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
            if (WidgetInstance)
            {
                WidgetInstance->AddToViewport();
                UE_LOG(LogTemp, Warning, TEXT("Widget added to viewport!"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance!"));
            }
            FInputModeGameAndUI InputMode;
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;
            UE_LOG(LogTemp, Warning, TEXT("Input mode set to UI Only and cursor enabled!"));
        }
       
    }
   //커스텀 종료시 후 이벤트 바인딩

    UKid_custom_ui* CustomUI = Cast<UKid_custom_ui>(WidgetInstance);
    if (CustomUI)
    {
        CustomUI->OnKidSetting.AddDynamic(this, &Akid_map_script::custom_finish);
        UE_LOG(LogTemp, Warning, TEXT("CustomUI event binding successful!"));
    }
	

    //각도조정
    // 현재 플레이어 컨트롤러 가져오기
    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        // 현재 Pawn 가져오기
        APawn* CurrentPawn = PlayerController->GetPawn();
        if (CurrentPawn)
        {
            // SpringArm 컴포넌트 찾기
            USpringArmComponent* SpringArm = CurrentPawn->FindComponentByClass<USpringArmComponent>();
            if (SpringArm)
            {
                // SpringArm의 상대 위치를 (0, 0, 52)로 설정
                SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 52.0f));
                // Target Arm Length를 100으로 설정
                SpringArm->TargetArmLength = 100.0f;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("SpringArm 컴포넌트를 찾을 수 없습니다."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Pawn을 찾을 수 없습니다."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController를 찾을 수 없습니다."));
    }



    //상대방그리기 (일단 나중에...)
    auto m_inst = Cast<UVirtual_life_GameInstance>(GetGameInstance());
    you_character = m_inst->draw_one_player(m_inst->m_marry->you_id);

    //너부터 춤춰라...
    if (you_character) {

        USkeletalMeshComponent* SkeletalMeshComp = you_character->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMeshComp) {
            UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/MaleStandingPose_UE_Montage.MaleStandingPose_UE_Montage")), 1.0f);
            }
        }
    }
    //나도 출게...
    ACharacter* AOwner = Cast<ACharacter>( PlayerController->GetPawn());
    if (AOwner) {
        USkeletalMeshComponent* SkeletalMeshComp = AOwner->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMeshComp) {
            UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
            if (AnimInstance)
            {
                AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/MaleStandingPose2_UE_Montage.MaleStandingPose2_UE_Montage")), 1.0f);
            }
        }
    }

    PrimaryActorTick.bCanEverTick = true;
}

void Akid_map_script::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds); // 부모 클래스의 Tick 호출
 
    if (you_character) {
        Um_CustomizableSkeletalComponent* customizable = you_character->FindComponentByClass<Um_CustomizableSkeletalComponent>();
        if (customizable) {
            if (customizable->GetCustomizableObjectInstance()) {
                customizable->feel_change(F_SMILE);
                PrimaryActorTick.bCanEverTick = false;
                
           }
        }  
    }
    


}

void Akid_map_script::custom_finish(float g_value, uint8 per_value, FString hello,FString name)
{
    
    UE_LOG(LogTemp, Warning, TEXT("hello"));
        if (WidgetInstance)
        {
            UKid_custom_ui* CustomUI = Cast<UKid_custom_ui>(WidgetInstance);
            if (CustomUI)
            {
                CustomUI->NativeDestruct();
            }
            WidgetInstance->RemoveFromParent();
            WidgetInstance = nullptr;
        }
    
}
