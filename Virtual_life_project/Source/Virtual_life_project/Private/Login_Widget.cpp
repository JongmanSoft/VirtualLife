// Fill out your copyright notice in the Description page of Project Settings.


#include "Login_Widget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Virtual_life_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULogin_Widget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Login_Btn)
    {
        Login_Btn->OnClicked.AddDynamic(this, &ULogin_Widget::OnLoginClicked);
    }

    if (ID_ETB)
    {
        ID_ETB->OnTextChanged.AddDynamic(this, &ULogin_Widget::OnIDTextChanged);
    }
}

void ULogin_Widget::OnLoginClicked()
{
    if (ID_ETB && Password_ETB)
    {
        FString ID = ID_ETB->GetText().ToString();
        FString Password = Password_ETB->GetText().ToString();

        UE_LOG(LogTemp, Log, TEXT("Login - ID: %s, PW: %s"), *ID, *Password);

        // 로그인 요청
        auto GameInstance = Cast<UVirtual_life_GameInstance>(UGameplayStatics::GetGameInstance(this));
        if (GameInstance) { 
            GameInstance->ConnectServer(Password); // 임시로 패스워드에 ip 입력하게 함
            GameInstance->SendLoginInfoPacket(ID, "0000");
        }
    }
}

void ULogin_Widget::OnIDTextChanged(const FText& Text)
{
    FString Input = Text.ToString();
    FString Filtered;

    // 알파벳만 허용
    for (int32 i = 0; i < Input.Len(); ++i)
    {
        TCHAR c = Input[i];
        if (FChar::IsAlpha(c))
        {
            Filtered.AppendChar(c);
        }

        // 최대 20자 제한
        if (Filtered.Len() >= 20)
            break;
    }

    if (Filtered != Input)
    {
        ID_ETB->SetText(FText::FromString(Filtered));
    }
}
