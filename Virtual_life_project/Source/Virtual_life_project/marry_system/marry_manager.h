// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kid_custom_ui.h"
#include "../../server/protocol.h"
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
	UPROPERTY(BlueprintReadWrite, Category = "marry")
	int my_id;
	UPROPERTY(BlueprintReadWrite, Category = "marry")
	int you_id;

	// 자식 커스텀 맵에서 얻은정보
	Customizing kid_cinfo;
	uint8 Personality;
	FString hello_text;
	FString kid_name; //자식이름
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "marry")
	float kid_x;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "marry")
	float kid_y;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "marry")
	float kid_z;

	UPROPERTY(BlueprintReadWrite, Category = "marry")
	bool have_to_send_kid = false;

	//유도 정보
	float gentic_offset;

public:
	void set_kid_custom_data(Customizing kid, uint8 personality, FString input_text, FString name);
	
	UFUNCTION(BlueprintCallable)
	void send_add_kid();
};
