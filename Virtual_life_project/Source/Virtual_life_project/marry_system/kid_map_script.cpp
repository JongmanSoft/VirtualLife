// Fill out your copyright notice in the Description page of Project Settings.


#include "kid_map_script.h"
#include "Blueprint/UserWidget.h"
#include "../Custom/m_CustomizableSkeletalComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "kid_npc_actor.h"
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
               
            }
           
            FInputModeGameAndUI InputMode;
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;
       
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
//    you_character = m_inst->draw_one_player(m_inst->m_marry->you_id); //여기수정

    //너부터 춤춰라...
    if (you_character)
    {
        USkeletalMeshComponent* SkeletalMeshComp = you_character->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMeshComp)
        {
            UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
            if (AnimInstance)
            {
                // 애니메이션 시퀀스 로드
                UAnimSequence* AnimSequence = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/animation/animation_resource/MaleStandingPose_UE.MaleStandingPose_UE"));
                if (AnimSequence)
                {
                    // 애니메이션 모드를 Use Animation Asset으로 설정
                    SkeletalMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
                    // 애니메이션 시퀀스 재생
                    AnimInstance->PlaySlotAnimationAsDynamicMontage(AnimSequence, "DefaultSlot", 0.25f, 0.25f, 1.0f);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to load AnimSequence"));
                }
            }
        }
    }
    //나도 출게...
    ACharacter* AOwner = Cast<ACharacter>(PlayerController->GetPawn());
    if (AOwner)
    {
        USkeletalMeshComponent* SkeletalMeshComp = AOwner->FindComponentByClass<USkeletalMeshComponent>();
        if (SkeletalMeshComp)
        {
            UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
            if (AnimInstance)
            {
                UE_LOG(LogTemp, Warning, TEXT("PAWN_ANIMATION"));
                // 애니메이션 시퀀스 로드
                UAnimSequence* AnimSequence = LoadObject<UAnimSequence>(nullptr, TEXT("/Game/animation/animation_resource/MaleStandingPose2_UE.MaleStandingPose2_UE"));
                if (AnimSequence)
                {
                    // 애니메이션 모드를 Use Animation Asset으로 설정
                    SkeletalMeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
                    // 애니메이션 시퀀스 재생
                    AnimInstance->PlaySlotAnimationAsDynamicMontage(AnimSequence, "DefaultSlot", 0.25f, 0.25f, 1.0f);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to load AnimSequence"));
                }
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

        //커스텀 시작! 0일수록 나, 1일수록 상대방임
        auto m_inst = Cast<UVirtual_life_GameInstance>(GetGameInstance());
		if (m_inst)
		{
            //auto you_custom = m_inst->OtherPlayers[m_inst->m_marry->you_id].cinfo; //여기수정
            Customizing you_custom;
            Customizing my_custom;
			m_inst->custom_packet_setup(my_custom, m_inst->m_custom);

            if (g_value < 0.5) {
                //얼굴이 나에 가깝다
				set_customizing_inteerpol(you_custom, my_custom, g_value);
            }
            else if (g_value > 0.5) {
                //얼굴이 상대방에 가깝다
				set_customizing_inteerpol(my_custom, you_custom, 1 - g_value);
            }

            //그럼 결국 자식 커스텀은..
			Customizing kid_custom = make_kid_customizing(my_custom,my_custom); //여기수정
            m_inst->m_marry->set_kid_custom_data(kid_custom, per_value, hello, name);

            if (preview_kid)
            {
                Akid_npc_actor* npc_kid = Cast<Akid_npc_actor>(preview_kid);
                if (npc_kid) npc_kid->set_new_custom(kid_custom);
                APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
                PlayerController->SetViewTargetWithBlend(preview_kid, 2.0f);

                make_finish_widget(name, per_value, hello);

            }
		}

}

void Akid_map_script::set_customizing_inteerpol(Customizing& change_cus, const Customizing& standard_cus, float interpol_value)
{
    change_cus.skin = change_cus.skin * interpol_value + standard_cus.skin * (1 - interpol_value);
    change_cus.R_eye_color_hue = change_cus.R_eye_color_hue * interpol_value + standard_cus.R_eye_color_hue * (1 - interpol_value);
    change_cus.R_eye_color_sat = change_cus.R_eye_color_sat * interpol_value + standard_cus.R_eye_color_sat * (1 - interpol_value);
    change_cus.L_eye_color_hue = change_cus.L_eye_color_hue * interpol_value + standard_cus.L_eye_color_hue * (1 - interpol_value);
    change_cus.L_eye_color_sat = change_cus.L_eye_color_sat * interpol_value + standard_cus.L_eye_color_sat * (1 - interpol_value);
    change_cus.eye_scale = change_cus.eye_scale * interpol_value + standard_cus.eye_scale * (1 - interpol_value);
    change_cus.pupil_scale = change_cus.pupil_scale * interpol_value + standard_cus.pupil_scale * (1 - interpol_value);
    change_cus.hair_color_R = change_cus.hair_color_R * interpol_value + standard_cus.hair_color_R * (1 - interpol_value);
    change_cus.hair_color_G = change_cus.hair_color_G * interpol_value + standard_cus.hair_color_G * (1 - interpol_value);
    change_cus.hair_color_B = change_cus.hair_color_B * interpol_value + standard_cus.hair_color_B * (1 - interpol_value);
    change_cus.eye_width = change_cus.eye_width * interpol_value + standard_cus.eye_width * (1 - interpol_value);
    change_cus.eye_thick = change_cus.eye_thick * interpol_value + standard_cus.eye_thick * (1 - interpol_value);
    change_cus.eye_slope = change_cus.eye_slope * interpol_value + standard_cus.eye_slope * (1 - interpol_value);
    change_cus.nose_width = change_cus.nose_width * interpol_value + standard_cus.nose_width * (1 - interpol_value);
    change_cus.nose_height = change_cus.nose_height * interpol_value + standard_cus.nose_height * (1 - interpol_value);
    change_cus.mouse_width = change_cus.mouse_width * interpol_value + standard_cus.mouse_width * (1 - interpol_value);
    change_cus.mouse_thick = change_cus.mouse_thick * interpol_value + standard_cus.mouse_thick * (1 - interpol_value);
    change_cus.mouse_slope = change_cus.mouse_slope * interpol_value + standard_cus.mouse_slope * (1 - interpol_value);
    change_cus.chin = change_cus.chin * interpol_value + standard_cus.chin * (1 - interpol_value);
    change_cus.jaw = change_cus.jaw * interpol_value + standard_cus.jaw * (1 - interpol_value);
    change_cus.heavy = change_cus.heavy * interpol_value + standard_cus.heavy * (1 - interpol_value);
    change_cus.face_width = change_cus.face_width * interpol_value + standard_cus.face_width * (1 - interpol_value);
}

Customizing Akid_map_script::make_kid_customizing(const Customizing& my_custom, const Customizing& you_custom)
{
	Customizing kid_custom;
	float random_value = FMath::FRandRange(0.0f, 1.0f); 

    kid_custom.skin = my_custom.skin * random_value + you_custom.skin * (1-random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
	kid_custom.R_eye_color_hue = my_custom.R_eye_color_hue * random_value + you_custom.R_eye_color_hue * (1 - random_value);
	random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.R_eye_color_sat = my_custom.R_eye_color_sat * random_value + you_custom.R_eye_color_sat * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.L_eye_color_hue = my_custom.L_eye_color_hue * random_value + you_custom.L_eye_color_hue * (1 - random_value);
	random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.L_eye_color_sat = my_custom.L_eye_color_sat * random_value + you_custom.L_eye_color_sat * (1 - random_value);
	random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.eye_scale = my_custom.eye_scale * random_value + you_custom.eye_scale * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.pupil_scale = my_custom.pupil_scale * random_value + you_custom.pupil_scale * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.hair_color_R = my_custom.hair_color_R * random_value + you_custom.hair_color_R * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.hair_color_G = my_custom.hair_color_G * random_value + you_custom.hair_color_G * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.hair_color_B = my_custom.hair_color_B * random_value + you_custom.hair_color_B * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.eye_width = my_custom.eye_width * random_value + you_custom.eye_width * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.eye_thick = my_custom.eye_thick * random_value + you_custom.eye_thick * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.eye_slope = my_custom.eye_slope * random_value + you_custom.eye_slope * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.nose_width = my_custom.nose_width * random_value + you_custom.nose_width * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.nose_height = my_custom.nose_height * random_value + you_custom.nose_height * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.mouse_width = my_custom.mouse_width * random_value + you_custom.mouse_width * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.mouse_thick = my_custom.mouse_thick * random_value + you_custom.mouse_thick * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.mouse_slope = my_custom.mouse_slope * random_value + you_custom.mouse_slope * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.chin = my_custom.chin * random_value + you_custom.chin * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.jaw = my_custom.jaw * random_value + you_custom.jaw * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.heavy = my_custom.heavy * random_value + you_custom.heavy * (1 - random_value);
    random_value = FMath::FRandRange(0.0f, 1.0f);
    kid_custom.face_width = my_custom.face_width * random_value + you_custom.face_width * (1 - random_value);

	//헤어는 랜덤으로
	kid_custom.hair = FMath::RandRange(0, 4);
	//옷은 랜덤으로   
	kid_custom.shirt = FMath::RandRange(0, 1);
	kid_custom.pants = FMath::RandRange(0, 1);
	kid_custom.shoes = FMath::RandRange(0, 1);
	//눈썹과 안경은 랜덤으로
	kid_custom.eyebrows = FMath::RandRange(0, 2);

    
    return kid_custom;
}

void Akid_map_script::make_finish_widget(const FString& kid_name, const int& personality, const FString& hello_text)
{
    // 블루프린트 위젯 클래스 동적 로드
    UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/kid_finish_ui.kid_finish_ui_C"));
    if (WidgetClass)
    {
        // 현재 월드와 플레이어 컨트롤러 가져오기
        UWorld* World = GetWorld();
        if (World)
        {
            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
            if (PlayerController)
            {
                // 위젯 인스턴스 생성
                UUserWidget* MyWidget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
                if (MyWidget)
                {
                    // kid_name 설정
                    FProperty* Property = MyWidget->GetClass()->FindPropertyByName(FName("kid_name"));
                    if (Property)
                    {
                        FString* kid_name_value = Property->ContainerPtrToValuePtr<FString>(MyWidget);
                        if (kid_name_value)
                        {
                            *kid_name_value = kid_name;
                        }
                    }

                    // kid_hello 설정
                    Property = MyWidget->GetClass()->FindPropertyByName(FName("kid_hello"));
                    if (Property)
                    {
                        FString* kid_hello_value = Property->ContainerPtrToValuePtr<FString>(MyWidget);
                        if (kid_hello_value)
                        {
                            *kid_hello_value = hello_text;
                        }
                    }

                    // kid_personality 설정
                    Property = MyWidget->GetClass()->FindPropertyByName(FName("kid_personality"));
                    if (Property)
                    {
                        int* kid_per_value = Property->ContainerPtrToValuePtr<int>(MyWidget);
                        if (kid_per_value)
                        {
                            *kid_per_value = personality;
                        }
                    }

                    // 뷰포트에 추가
                    MyWidget->AddToViewport();
                }
            }
        }
    }
}