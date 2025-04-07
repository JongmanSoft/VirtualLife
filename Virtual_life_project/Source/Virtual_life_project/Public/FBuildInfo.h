// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FBuildInfo.generated.h"

UENUM(BlueprintType)
enum class EBuildCategories : uint8
{
    Furniture,
    Bedroom,
    Kitchen,
    Storage,
    InteriorDecor,
    Decor
};

USTRUCT(BlueprintType)
struct FBuildInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EBuildCategories Categories;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Image;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* Mesh;
};