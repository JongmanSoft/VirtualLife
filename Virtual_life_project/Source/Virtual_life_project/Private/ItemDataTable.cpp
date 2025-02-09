#include "ItemDataTable.h"
#include "Engine/DataTable.h"

void UItemDataTable::LoadItemData()
{
    // ������ ���̺� �ε� �� ������ Ȯ�� ����
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
