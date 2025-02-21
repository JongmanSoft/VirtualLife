// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Custom_data.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VIRTUAL_LIFE_PROJECT_API UCustom_data : public UObject
{
	GENERATED_BODY()
	
public:

	UCustom_data();

public:
	//커스텀 데이터들
	float skin; // 피부색
	unsigned short shirt;
	unsigned short pants;
	unsigned short shoes;
	//눈정보 
	float R_eye_color_hue;
	float R_eye_color_sat;
	float L_eye_color_hue;
	float L_eye_color_sat;
	float eye_scale;
	float pupil_scale;
	//헤어정보
	unsigned short hair;
	float hair_color_R;
	float hair_color_G;
	float hair_color_B;




};
