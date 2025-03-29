// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
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
    AActor* m_owner;

    // 마우스 클릭 처리 함수
    void OnLeftMouseClick();
};