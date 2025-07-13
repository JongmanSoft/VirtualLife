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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkinData")
	float skin; // 피부색
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClothigData")
	uint8 shirt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClothigData")
	uint8 pants;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClothigData")
	uint8 shoes;
	//눈정보 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeData")
	float R_eye_color_hue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeData")
	float R_eye_color_sat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeData")
	float L_eye_color_hue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeData")
	float L_eye_color_sat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeData")
	float eye_scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeData")
	float pupil_scale;
	//헤어정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairData")
	uint8 hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairData")
	float hair_color_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairData")
	float hair_color_G;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairData")
	float hair_color_B;

	//얼굴 커스텀
	//눈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeData")
	float eye_width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeData")
	float eye_thick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeData")
	float eye_slope;
	//코
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NoseData")
	float nose_width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NoseData")
	float nose_height;
	//입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseData")
	float mouse_width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseData")
	float mouse_thick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MouseData")
	float mouse_slope;
	//얼굴형
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FaceData")
	float chin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FaceData")
	float jaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FaceData")
	float heavy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FaceData")
	float face_width;

	//아이브로우, 마스카라
	unsigned short eyebrows;
	unsigned short glasses;
};
