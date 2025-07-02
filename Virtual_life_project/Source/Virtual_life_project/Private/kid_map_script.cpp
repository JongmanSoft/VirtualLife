// Fill out your copyright notice in the Description page of Project Settings.


#include "kid_map_script.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

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
            // 위젯 생성 및 뷰포트에 추가
            UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
            if (WidgetInstance)
            {
                WidgetInstance->AddToViewport();
                UE_LOG(LogTemp, Warning, TEXT("Widget added to viewport!"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance!"));
            }

            // 인풋 모드를 UI Only로 설정
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(WidgetInstance->TakeWidget());
            PlayerController->SetInputMode(InputMode);

            // 마우스 커서 표시
            PlayerController->bShowMouseCursor = true;
            UE_LOG(LogTemp, Warning, TEXT("Input mode set to UI Only and cursor enabled!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load widget class!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
    }
    
}

void Akid_map_script::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds); // 부모 클래스의 Tick 호출
    UE_LOG(LogTemp, Warning, TEXT("Level Tick triggered! DeltaTime: %f"), DeltaSeconds);


}
