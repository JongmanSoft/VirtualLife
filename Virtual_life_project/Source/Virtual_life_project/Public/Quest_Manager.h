// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest.h"
#include "Quest_Manager.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class VIRTUAL_LIFE_PROJECT_API UQuest_Manager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TMap <uint8 , UQuest*> Quests; //퀘스트의 각 아이디에 대해서 매핑, 없으면 nullptr

public:
	//서버에서 퀘스트들을 받아 초기화하는 함수
	//void Load_Quest();
	
	//퀘스트를 추가
	UFUNCTION(BlueprintCallable)
	void ADD_QUEST(const uint8& quest_id);
	//퀘스트 완료했니?
	UFUNCTION(BlueprintCallable)
	bool Quest_complete(const uint8& quest_id);
	//퀘스트 삭제
	UFUNCTION(BlueprintCallable)
	void Delete_Quest(const uint8& quest_id);
	
	UFUNCTION(BlueprintCallable)
	void finish_Quest(const uint8& quest_id);

	UFUNCTION(BlueprintCallable)
	void Done_Process(const uint8& quest_id); // 퀘스트가 완료된 후의 처리

	UFUNCTION(BlueprintCallable)
	int32 get_quests_count();
};
