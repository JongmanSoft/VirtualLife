// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EBuildCategories.generated.h"

UENUM(BlueprintType)
enum class EBuildCategories : uint8
{
    Desk001     UMETA(DisplayName = "Desk001"),
    Desk002     UMETA(DisplayName = "Desk002"),
    Desk003     UMETA(DisplayName = "Desk003"),
    Chair001    UMETA(DisplayName = "Chair001")
};
