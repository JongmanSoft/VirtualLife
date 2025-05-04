// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MuCO/CustomizableObject.h"
#include "MuCO/CustomizableObjectInstance.h"
#include "MuCO/CustomizableSkeletalComponent.h"
#include "MuCO/CustomizableObjectSystem.h" 

#include "../Network/NetworkManager.h"
#include "m_CustomizableSkeletalComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VIRTUAL_LIFE_PROJECT_API Um_CustomizableSkeletalComponent : public UCustomizableSkeletalComponent
{
	GENERATED_BODY()

public:
	Um_CustomizableSkeletalComponent();
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
public:
	//매핑정보
	//상의
	FString torso_str[3] = {"Shirt","Tshirt","NULL"};
	//하의
	FString pants_str[2] = { "short_pants","long_pants" };
	//신발
	FString shoes_str[2] = { "ropper","boots" };

public:
	//커스텀 데이터들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkinData")
	float skin; // 피부색
	// 옷 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClothigData")
	uint8 shirt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClothigData")
	uint8 pants;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClothigData")
	uint8 shoes;
	// 눈 정보
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

	// 헤어 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairData")
	uint8 hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairData")
	float hair_color_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairData")
	float hair_color_G;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HairData")
	float hair_color_B;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* MI_hair = nullptr;


	//브로우, 마스카라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EYEgroomData")
	uint8 eyebrows;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EYEgroomData")
	uint8 glasses;

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


public:
	//인스턴스 불러오기 
	UFUNCTION(BlueprintCallable, Category = "update")
	void instance_import();
	//인스턴스로 내보내기 
	UFUNCTION(BlueprintCallable, Category = "update")
	void instance_export();
	//커스텀데이터 내보내기용
	void custom_export(UCustom_data* cus);
	//커스텀데이터 초기화용(게임플레이어용)
	void generated_custom(const UCustom_data* cus);
	//액터에 변경 적용함수(서버용)
	void custom_data_update(const Customizing cus);
	UFUNCTION(BlueprintCallable, Category = "update")
	void apply_actor_custom();
	
	UFUNCTION(BlueprintCallable, Category = "update")
	void random_custom();
	
};
