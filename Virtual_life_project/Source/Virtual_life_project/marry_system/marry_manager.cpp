// Fill out your copyright notice in the Description page of Project Settings.


#include "marry_manager.h"
#include "Virtual_life_GameInstance.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Math/UnrealMathUtility.h"



void Umarry_manager::set_kid_custom_data(Customizing kid, uint8 personality, FString input_text, FString name)
{
	kid_cinfo = kid; // 자식 커스터마이징 정보	
	Personality = personality;
	hello_text = input_text;
	kid_name = name;
	have_to_send_kid = true; // 자식 정보를 보내야 함
}

void Umarry_manager::send_add_kid()
{
	have_to_send_kid = false;
	auto m_inst = Cast<UVirtual_life_GameInstance>(GetWorld()->GetGameInstance());

	m_inst->SendCreateKidPacket(0, 0, kid_cinfo, kid_x, kid_y, kid_z, 
		FMath::FRandRange(0.0f, 360.0f), Personality,kid_name,hello_text);
	
}
