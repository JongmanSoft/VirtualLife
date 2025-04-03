// Fill out your copyright notice in the Description page of Project Settings.

#include "custom_preview_handler.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"

// Sets default values for this component's properties
Ucustom_preview_handler::Ucustom_preview_handler()
{
    PrimaryComponentTick.bCanEverTick = true;
   
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
    PlayerController = GetWorld()->GetFirstPlayerController();
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
            InputComp->BindAction("LeftMouseButton", IE_Released, this, &Ucustom_preview_handler::OffLeftMouseClick);
            InputComp->BindAxis("Mouse_X", this, &Ucustom_preview_handler::move_mouse_x);
            InputComp->BindAxis("Mouse_Y", this, &Ucustom_preview_handler::move_mouse_y);
        }
           
            InputComp->Priority = 10;
          
     }
       
  }
    





// 마우스 클릭 처리
void Ucustom_preview_handler::OnLeftMouseClick()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;



    FHitResult HitResult;
    if (PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult))
    {
        PC->bShowMouseCursor = true;
        UPrimitiveComponent* HitComponent = HitResult.GetComponent();
        if (HitComponent && !click)
        {
           // DrawDebugPoint(GetWorld(), HitResult.Location, 10.f, FColor::Green, false, 2.f);
            _drag_stack = 0.0;
            _click_stack = 0.0;
            PlayerController->GetMousePosition(start_mouse_x,start_mouse_y);
            current_mouse_x = start_mouse_x;
            current_mouse_y = start_mouse_y;
            click = true;
            eye_click = HitComponent->ComponentHasTag(FName("eye"));
            if (eye_click) {
                start_value_1 = m_custom->eye_width;
                start_value_2 = m_custom->eye_thick;
                m_custom->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("preview_texture_enum"), "P_EYE", -1);
                return;
            }

            eye_slope_click = HitComponent->ComponentHasTag(FName("eye_slope"));
            if (eye_slope_click) {
                start_value_1 = m_custom->eye_slope;
                m_custom->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("preview_texture_enum"), "P_EYE", -1);
                return;
            } 

            mouth_click = HitComponent->ComponentHasTag(FName("mouse"));
            if (mouth_click) {
                start_value_1 = m_custom->mouse_width;
                start_value_2 = m_custom->mouse_thick;
                m_custom->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("preview_texture_enum"), "P_MOUTH", -1);
                return;
            }


            mouth_slope_click = HitComponent->ComponentHasTag(FName("mouse_slope"));
            if (mouth_slope_click) {
                start_value_1 = m_custom->mouse_slope;
                m_custom->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("preview_texture_enum"), "P_MOUTH", -1);
                return;
            }

            nose_click = HitComponent->ComponentHasTag(FName("nose"));
            if (nose_click) {
                start_value_1 = m_custom->nose_width;
                start_value_2 = m_custom->nose_height;
                m_custom->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("preview_texture_enum"), "P_NOSE", -1);
                return;
            }

            chin_click = HitComponent->ComponentHasTag(FName("chin"));  
            if (chin_click) {
                start_value_1 = m_custom->chin;
                m_custom->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("preview_texture_enum"), "P_JAW", -1);
                return;
            }

            jaw_click = HitComponent->ComponentHasTag(FName("jaw"));
            if (jaw_click) {
                start_value_1 = m_custom->jaw;
                m_custom->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("preview_texture_enum"), "P_JAW", -1);
                return;
            } 

            forehead_click = HitComponent->ComponentHasTag(FName("forehead"));
            if (forehead_click) {
                start_value_1 = m_custom->face_width;
                start_value_2 = m_custom->heavy;
                m_custom->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("preview_texture_enum"), "P_FACE", -1);
                return;
            }
            
        }
    }
   

}

void Ucustom_preview_handler::OffLeftMouseClick()
{
    click = false;
    eye_click = false;
    eye_slope_click = false;
    mouth_click = false;
    mouth_slope_click = false;
    nose_click = false;
    chin_click = false;
    jaw_click = false;
    forehead_click = false;
    m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Stack_click_time"), 0.0, -1);
    m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Drag_dist"), 0.0, -1);
    PrimaryComponentTick.bCanEverTick = true;
    m_custom->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("preview_texture_enum"), "NULL", -1);
    m_custom->apply_actor_custom();
}

void Ucustom_preview_handler::move_mouse_x(float delta)
{
    current_mouse_x += delta * click;
}

void Ucustom_preview_handler::move_mouse_y(float delta)
{
   
    current_mouse_y += delta * click;
}

void Ucustom_preview_handler::m_tick(float delta)
{
    if (!click)return;
    _click_stack += 0.1* delta;
    m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Stack_click_time"), _click_stack, -1);
    float x_distance = FMath::Clamp(start_mouse_x - current_mouse_x,-30,30)/30.0;
    float y_distance = FMath::Clamp(start_mouse_y - current_mouse_y, -30, 30)/30.0;

    UE_LOG(LogTemp, Log, TEXT("distance_x: %f"), x_distance);

    if (eye_click) { 
        m_custom->eye_width = start_value_1 + x_distance;
        m_custom->eye_thick = start_value_2 - y_distance;

        _drag_stack = (FMath::Abs(x_distance) + FMath::Abs(y_distance)) / 2;
        m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Drag_dist"), _drag_stack, -1);
        m_custom->apply_actor_custom();
        return;
    }

    if (eye_slope_click) {
        m_custom->eye_slope = start_value_1 - y_distance;
        

        _drag_stack = FMath::Abs(y_distance);
        m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Drag_dist"), _drag_stack, -1);
        
        m_custom->apply_actor_custom();
        return;
    }
    if (mouth_click) {
        m_custom->mouse_width = start_value_1 + x_distance;
        m_custom->mouse_thick = start_value_2 + y_distance;

        _drag_stack = (FMath::Abs(x_distance) + FMath::Abs(y_distance)) / 2;
        m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Drag_dist"), _drag_stack, -1);

        m_custom->apply_actor_custom();
        return;
    }
    if (mouth_slope_click) {
        m_custom->mouse_slope = start_value_1 - y_distance;

        _drag_stack = FMath::Abs(y_distance);
        m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Drag_dist"), _drag_stack, -1);

        m_custom->apply_actor_custom();
        return;
    }
    if (nose_click) {
        m_custom->nose_width = start_value_1 + x_distance;
        m_custom->nose_height = start_value_2 - y_distance;

        _drag_stack = (FMath::Abs(x_distance) + FMath::Abs(y_distance)) / 2;
        m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Drag_dist"), _drag_stack, -1);

        m_custom->apply_actor_custom();
        return;
    }
    if (chin_click) {
        m_custom->chin = start_value_1 + y_distance;

        _drag_stack = FMath::Abs(y_distance);
        m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Drag_dist"), _drag_stack, -1);

        m_custom->apply_actor_custom();
        return;
    }
    if (jaw_click) {
        m_custom->jaw = start_value_1 + x_distance;

        _drag_stack = FMath::Abs(y_distance);
        m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Drag_dist"), _drag_stack, -1);

        m_custom->apply_actor_custom();
        return;
    }
    if (forehead_click) {
        m_custom->face_width = start_value_1 + x_distance;
        m_custom->heavy = start_value_2 + y_distance;

        _drag_stack = (FMath::Abs(x_distance) + FMath::Abs(y_distance)) / 2;
        m_custom->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Drag_dist"), _drag_stack, -1);

        m_custom->apply_actor_custom();
        return;
    }

}

