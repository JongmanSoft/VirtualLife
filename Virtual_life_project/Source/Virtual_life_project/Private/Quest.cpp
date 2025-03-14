// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"

bool UQuest::QuestComplete()
{

	for (FWriteRequirValue V : RequirValues) {
		if (*V.RequirBindValue < (int32)V.RequirCount) return false;
	}
	return true;
}

TArray<FReadRequirValue> UQuest::GetRequirValues() const
{
    TArray<FReadRequirValue> Result; // 반환할 배열
    Result.Reserve(RequirValues.Num()); // 메모리 할당 최적화

    for (const FWriteRequirValue& WriteReq : RequirValues)
    {
        FReadRequirValue ReadReq;
        ReadReq.RequirCount = WriteReq.RequirCount; // 카운트 복사
        ReadReq.RequirBindValue = (WriteReq.RequirBindValue != nullptr) ? *WriteReq.RequirBindValue : 0; // 포인터 값 복사, null 체크 포함
        Result.Add(ReadReq);
    }

    return Result;
}
