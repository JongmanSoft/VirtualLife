// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "m_CustomizableSkeletalComponent.h"
#include "custom_preview_handler.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VIRTUAL_LIFE_PROJECT_API Ucustom_preview_handler : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    Ucustom_preview_handler();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // PlayerController의 입력 바인딩 설정
    virtual void SetupInputComponent();

  
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "preview_custom")
    Um_CustomizableSkeletalComponent* m_custom;
public:
    AActor* m_owner;
    APlayerController* PlayerController;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mouse")
    bool click = false;
    bool eye_click = false;
    bool eye_slope_click = false;
    bool mouth_click = false;
    bool mouth_slope_click = false;
    bool nose_click = false;
    bool chin_click = false;
    bool jaw_click = false;
    bool forehead_click = false;

    float start_value_1 = 0;
    float start_value_2 = 0;


    float start_mouse_x = 0;
    float start_mouse_y = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mouse")
    float current_mouse_x = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mouse")
    float current_mouse_y = 0;

    // 마우스 클릭 처리 함수
    void OnLeftMouseClick();
    //클릭해제
    void OffLeftMouseClick();
    //마우스 이동 함수
    void move_mouse_x(float delta);
    void move_mouse_y(float delta);
    
    //임시틱
    UFUNCTION(BlueprintCallable)
    void m_tick();

};