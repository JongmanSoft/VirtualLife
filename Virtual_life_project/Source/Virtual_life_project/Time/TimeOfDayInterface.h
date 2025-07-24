// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TimeOfDayInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTimeOfDayInterface : public UInterface
{
    GENERATED_BODY()
};

class VIRTUAL_LIFE_PROJECT_API ITimeOfDayInterface
{
    GENERATED_BODY()

public:

    // 블루프린트에서 오버라이드 가능 + C++ 구현도 가능
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TimeOfDay")
    void NotifySunrise();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TimeOfDay")
    void NotifySunset();
};
