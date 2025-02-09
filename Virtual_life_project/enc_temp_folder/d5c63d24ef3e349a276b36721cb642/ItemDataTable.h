#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "ItemDataTable.generated.h"

// 아이템 종류를 나타내는 열거형 선언
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
    Item UMETA(DisplayName = "Item"),
    Equipment UMETA(DisplayName = "Equipment"),
    Clothing UMETA(DisplayName = "Clothing"),
    Furniture UMETA(DisplayName = "Furniture")
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    uint8 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemCategory ItemCategory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemDesc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UStaticMesh* ItemMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemPrice;
};

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UItemDataTable : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
    UDataTable* ItemDataTable;

    // 함수 선언
    UFUNCTION(BlueprintCallable, Category = "Data")
    void LoadItemData();
};
