// Fill out your copyright notice in the Description page of Project Settings.


#include "m_CustomizableSkeletalComponent.h"

Um_CustomizableSkeletalComponent::Um_CustomizableSkeletalComponent() 
{

	//기본 디폴트 값 
	skin = 0.5;
	shirt = 0;
	pants = 0;
	shoes = 0;
	R_eye_color_hue = 1;
	R_eye_color_sat = 1;
	L_eye_color_hue = 1;
	L_eye_color_sat = 1;
	eye_scale = 0.35;
	pupil_scale = 0.9;
	hair = 0;
	hair_color_R = 1;
	hair_color_G = 0.2;
	hair_color_B = 1;
}

void Um_CustomizableSkeletalComponent::InitializeComponent()
{

	//액터가 월드에 배치될때 호출!

	Super::InitializeComponent();

	//동적인스턴스로 할당 
	UCustomizableObject* parentCO = LoadObject<UCustomizableObject>(nullptr, TEXT("/Game/custom_system/CO_metahuman.CO_metahuman"));
	if (parentCO)
	{
		// 새 인스턴스 생성
		UCustomizableObjectInstance* instance = NewObject<UCustomizableObjectInstance>(GetTransientPackage(), UCustomizableObjectInstance::StaticClass());
		instance->SetObject(parentCO);
		this->SetCustomizableObjectInstance(instance);
	}


}

void Um_CustomizableSkeletalComponent::custom_data_update(Customizing cus)
{
	skin =        cus.skin  ;
	shirt =       cus.shirt ;
	pants =       cus.pants ;
	shoes =       cus.shoes ;
	R_eye_color_hue =   cus.R_eye_color_hue;
	R_eye_color_sat =   cus.R_eye_color_sat;
	L_eye_color_hue =   cus.L_eye_color_hue;
	L_eye_color_sat =   cus.L_eye_color_sat;
	eye_scale =     cus.eye_scale;
	pupil_scale =   cus.pupil_scale;
	hair =          cus.hair;
	hair_color_R =  cus.hair_color_R;
	hair_color_G =  cus.hair_color_G;
	hair_color_B =  cus.hair_color_B;

	apply_actor_custom(); //변경 내용을 액터에 반영
}

void Um_CustomizableSkeletalComponent::apply_actor_custom()
{
	// IntParameters
	//추후 매핑 필요... 아직 미완
	this->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("Pants_enum"), FString("long_pants"), -1);
	this->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("shoes_enum"), FString("boots"), -1);
	this->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("Torso_enum"), FString("Tshirt"), -1);

	// FloatParameters
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_L_color_hue"), L_eye_color_hue, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_L_color_sat"), L_eye_color_sat, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_R_color_hue"), R_eye_color_hue, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_R_color_sat"), R_eye_color_sat, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_scale"), eye_scale, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("pupil_scale"), pupil_scale, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("skin_brighness"), skin, -1);


	//메시에 반영
	this->GetCustomizableObjectInstance()->UpdateSkeletalMeshAsync(false,false);
}
