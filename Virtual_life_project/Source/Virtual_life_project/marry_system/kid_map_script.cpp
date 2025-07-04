// Fill out your copyright notice in the Description page of Project Settings.


#include "kid_map_script.h"
#include "Blueprint/UserWidget.h"
#include "../Public/Virtual_life_GameInstance.h"
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
            FInputModeUIOnly InputMode;
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
	
    //부모 그리기
    auto m_inst = Cast<UVirtual_life_GameInstance>(GetGameInstance());
}

void Akid_map_script::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds); // 부모 클래스의 Tick 호출
    UE_LOG(LogTemp, Warning, TEXT("Level Tick triggered! DeltaTime: %f"), DeltaSeconds);


}

void Akid_map_script::custom_finish(float g_value, uint8 per_value, FString hello)
{
    
    UE_LOG(LogTemp, Warning, TEXT("hello"));
        if (WidgetInstance)
        {
            UKid_custom_ui* CustomUI = Cast<UKid_custom_ui>(WidgetInstance->GetWidgetFromName(TEXT("KidCustomUI")));
            if (CustomUI)
            {
                CustomUI->NativeDestruct();
            }
            WidgetInstance->RemoveFromViewport();
            WidgetInstance = nullptr;
        }
    
}
