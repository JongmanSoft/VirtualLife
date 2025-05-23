// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameMode.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

void ALoginGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (LoginUIClass)
    {
        UUserWidget* LoginUI = CreateWidget<UUserWidget>(GetWorld(), LoginUIClass);
        if (LoginUI)
        {
            LoginUI->AddToViewport();
        }
    }
}
