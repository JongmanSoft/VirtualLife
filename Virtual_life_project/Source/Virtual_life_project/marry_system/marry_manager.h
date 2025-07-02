// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kid_custom_ui.h"
#include "marry_manager.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API Umarry_manager : public UObject
{
	GENERATED_BODY()
public:


	//나, 상대방 아이디
	int my_id;
	int you_id;

	// 자식 커스텀 ui에서 받은 정보
	float genetic_value;
	uint8 Personality;
	FString hello_text;

	//유도 정보
	float gentic_offset;

public:
	void set_kid_custom_data(float _genetic_value, uint8 personality, FString input_text);
	
};
