#include "ItemDataTable.h"
#include "Engine/DataTable.h"

void UItemDataTable::LoadItemData()
{
    // 데이터 테이블 로드 및 데이터 확인 예제
    if (ItemDataTable)
    {
        TArray<FItemData*> Rows;
        ItemDataTable->GetAllRows<FItemData>(TEXT(""), Rows);

        for (auto& Row : Rows)
        {
            if (Row)
            {
                UE_LOG(LogTemp, Log, TEXT("ItemID: %d, ItemName: %s"), Row->ItemID, *Row->ItemName);
            }
        }
    }
}
