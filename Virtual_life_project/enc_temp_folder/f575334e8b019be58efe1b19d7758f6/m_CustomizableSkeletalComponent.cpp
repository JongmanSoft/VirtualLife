// Fill out your copyright notice in the Description page of Project Settings.


#include "m_CustomizableSkeletalComponent.h"
#include "Virtual_life_GameInstance.h"
#include "GroomComponent.h"

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
	eye_width =0;
	eye_thick =0;
	eye_slope=0;
	nose_width=0;
	nose_height=0;
	mouse_width=0;
	mouse_thick=0;
	mouse_slope=0;
	chin=0;
	jaw=0;
	heavy=0;
	face_width=0;

}

void Um_CustomizableSkeletalComponent::InitializeComponent()
{

	//Super::InitializeComponent();

	

}

void Um_CustomizableSkeletalComponent::BeginPlay()
{
	Super::BeginPlay();

	UCustomizableObject* parentCO = LoadObject<UCustomizableObject>(nullptr, TEXT("/Game/custom_system/CO_metahuman.CO_metahuman"));
	if (parentCO)
	{
		UCustomizableObjectInstance* instance = NewObject<UCustomizableObjectInstance>(this, UCustomizableObjectInstance::StaticClass());
		instance->SetObject(parentCO);
		this->SetCustomizableObjectInstance(instance);

	}


	this->SetComponentName(FName("Character"));


}

void Um_CustomizableSkeletalComponent::instance_import()
{
	UVirtual_life_GameInstance* game_inst = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
	if (game_inst && game_inst->m_custom) { // nullptr 체크
		generated_custom(game_inst->m_custom); // 포인터 역참조
	}
}

void Um_CustomizableSkeletalComponent::instance_export()
{
	UVirtual_life_GameInstance* game_inst = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());
	if (game_inst && game_inst->m_custom) { // nullptr 체크
		custom_export(game_inst->m_custom); // 포인터 역참조
	}
}

void Um_CustomizableSkeletalComponent::custom_export(UCustom_data* cus)
{
	cus->skin = skin;
	cus->shirt =shirt;
	cus->pants =pants;
	cus->shoes =shoes;
	cus->R_eye_color_hue = R_eye_color_hue;
	cus->R_eye_color_sat = R_eye_color_sat;
	cus->L_eye_color_hue = L_eye_color_hue;
	cus->L_eye_color_sat = L_eye_color_sat;
	cus->eye_scale =   eye_scale;
	cus->pupil_scale = pupil_scale;
	cus->hair = hair;
	cus->hair_color_R = hair_color_R;
	cus->hair_color_G = hair_color_G;
	cus->hair_color_B = hair_color_B;
	cus->eye_width = eye_width;
	cus->eye_thick = eye_thick;
	cus->eye_slope = eye_slope;
	cus->nose_width =  nose_width;
	cus->nose_height = nose_height;
	cus->mouse_width = mouse_width;
	cus->mouse_thick = mouse_thick;
	cus->mouse_slope = mouse_slope;
	cus->chin = chin;
	cus->jaw =  jaw;
	cus->heavy =heavy;
	cus->face_width = face_width;

}

void Um_CustomizableSkeletalComponent::generated_custom(const UCustom_data* cus)
{
	skin = cus->skin;
	shirt = cus->shirt;
	pants = cus->pants;
	shoes = cus->shoes;
	R_eye_color_hue = cus->R_eye_color_hue;
	R_eye_color_sat = cus->R_eye_color_sat;
	L_eye_color_hue = cus->L_eye_color_hue;
	L_eye_color_sat = cus->L_eye_color_sat;
	eye_scale = cus->eye_scale;
	pupil_scale = cus->pupil_scale;
	hair = cus->hair;
	hair_color_R = cus->hair_color_R;
	hair_color_G = cus->hair_color_G;
	hair_color_B = cus->hair_color_B;
	eye_width = cus->eye_width;
	eye_thick = cus->eye_thick;
	eye_slope = cus->eye_slope;
	nose_width =  cus->nose_width;
	nose_height = cus->nose_height;
	mouse_width = cus->mouse_width;
	mouse_thick = cus->mouse_thick;
	mouse_slope = cus->mouse_slope;
	chin = cus->chin;
	jaw = cus->jaw;
	heavy = cus->heavy;
	face_width = cus->face_width;
	apply_actor_custom(); //변경 내용을 액터에 반영
}

void Um_CustomizableSkeletalComponent::custom_data_update(const Customizing cus)
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
	eye_width = cus.eye_width;
	eye_thick = cus.eye_thick;
	eye_slope = cus.eye_slope;
	nose_width =  cus.nose_width;
	nose_height = cus.nose_height;
	mouse_width = cus.mouse_width;
	mouse_thick = cus.mouse_thick;
	mouse_slope = cus.mouse_slope;
	chin = cus.chin;
	jaw = cus.jaw;
	heavy = cus.heavy;
	face_width = cus.face_width;
	apply_actor_custom(); //변경 내용을 액터에 반영
}

void Um_CustomizableSkeletalComponent::apply_actor_custom()
{
	// IntParameters
	this->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("Pants_enum"), pants_str[pants], -1);
	this->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("shoes_enum"), shoes_str[shoes], -1);
	this->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(FString("Torso_enum"), torso_str[shirt], -1);

	// FloatParameters
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_L_color_hue"), L_eye_color_hue, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_L_color_sat"), L_eye_color_sat, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_R_color_hue"), R_eye_color_hue, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_R_color_sat"), R_eye_color_sat, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("eye_scale"), eye_scale, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("pupil_scale"), pupil_scale, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("skin_brighness"), skin, -1);

	// hair change
	// 오너 액터 가져오기
	AActor * OwnerActor = GetOwner();
	UGroomComponent* owner_hair = Cast<UGroomComponent>(OwnerActor->FindComponentByTag(UGroomComponent::StaticClass(), FName("HAIR")));
	if (owner_hair) {
		TCHAR groom_asset_file[3][100]
			= { TEXT("/Game/MetaHumans/woman/FemaleHair/Hair/Hair_M_BobMessy.Hair_M_BobMessy")
			,TEXT("/Game/MetaHumans/man/FemaleHair/Hair/Hair_S_Pixie.Hair_S_Pixie"),
			TEXT("/Game/MetaHumans/naked_character/MaleHair_fro/Hair/Hair_S_Casual.Hair_S_Casual")
		};
		TCHAR groom_binding_asset_file[3][100]
			= { TEXT("/Game/MetaHumans/woman/FemaleHair/GroomBinding/Hair_M_BobMessy_Binding.Hair_M_BobMessy_Binding")
			,TEXT("/Game/MetaHumans/man/FemaleHair/GroomBinding/Hair_S_Pixie_Binding.Hair_S_Pixie_Binding")
			,TEXT("/Game/MetaHumans/woman/FemaleHair/GroomBinding/Hair_M_BobMessy_Binding.Hair_M_BobMessy_Binding")
		};
		

		auto a = LoadObject<UGroomAsset>(nullptr, groom_asset_file[hair]);
		owner_hair->SetGroomAsset(a);
		owner_hair->SetBindingAsset(LoadObject<UGroomBindingAsset>(nullptr, groom_binding_asset_file[hair]));
	}

	// hair color change
	if (MI_hair)
	{
		MI_hair->SetScalarParameterValue(FName("hair_R"), hair_color_R);
		MI_hair->SetScalarParameterValue(FName("hair_G"), hair_color_G);
		MI_hair->SetScalarParameterValue(FName("hair_B"), hair_color_B);
	}


	// morph parameter apply
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Eye_slope"), eye_slope, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Eye_width"), eye_width, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Eye_thick"), eye_thick, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Nose_width"), nose_width, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Nose_height"), nose_height, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Mouse_width"), mouse_width, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Mouse_thick"), mouse_thick, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Mouse_slope"), mouse_slope, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Face_width"), face_width, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Heavy"), heavy, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Chin"), chin, -1);
	this->GetCustomizableObjectInstance()->SetFloatParameterSelectedOption(FString("Jaw"), jaw, -1);

	//메시에 반영
	this->GetCustomizableObjectInstance()->UpdateSkeletalMeshAsync(false,false);
	this->UpdateSkeletalMeshAsync();
}

void Um_CustomizableSkeletalComponent::random_custom()
{
	skin = FMath::RandRange(0.0, 1.0);
	shirt = 2;
	pants = 1;
	shoes = 0;
	R_eye_color_hue =  FMath::RandRange(0.0, 1.0);
	R_eye_color_sat =  FMath::RandRange(0.0, 1.0);
	L_eye_color_hue =  FMath::RandRange(0.0, 1.0);
	L_eye_color_sat =  FMath::RandRange(0.0, 1.0);
	eye_scale = FMath::RandRange(0.0, 1.0);
	pupil_scale = FMath::RandRange(0.0, 1.0);
	hair = FMath::RandRange(0,2);;
	hair_color_R = FMath::RandRange(0.0, 1.0);
	hair_color_G = FMath::RandRange(0.0, 1.0);
	hair_color_B = FMath::RandRange(0.0, 1.0);
	eye_width = FMath::RandRange(0.0, 1.0);
	eye_thick = FMath::RandRange(0.0, 1.0);
	eye_slope = FMath::RandRange(0.0, 1.0);
	nose_width = FMath::RandRange(0.0, 1.0);
	nose_height = FMath::RandRange(0.0, 1.0);
	mouse_width = FMath::RandRange(0.0, 1.0);
	mouse_thick = FMath::RandRange(0.0, 1.0);
	mouse_slope = FMath::RandRange(0.0, 1.0);
	chin = FMath::RandRange(0.0, 1.0);
	jaw = FMath::RandRange(0.0, 1.0);
	heavy = FMath::RandRange(0.0, 1.0);
	face_width = FMath::RandRange(0.0, 1.0);
	apply_actor_custom(); //변경 내용을 액터에 반영
}
