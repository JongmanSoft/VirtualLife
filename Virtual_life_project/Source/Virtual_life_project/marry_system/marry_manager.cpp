// Fill out your copyright notice in the Description page of Project Settings.


#include "marry_manager.h"
#include "Math/UnrealMathUtility.h"



void Umarry_manager::set_kid_custom_data(Customizing kid, uint8 personality, FString input_text, FString name)
{
	kid_cinfo = kid; // 자식 커스터마이징 정보	
	Personality = personality;
	hello_text = input_text;
	kid_name = name;
	have_to_send_kid = true; // 자식 정보를 보내야 함
}
