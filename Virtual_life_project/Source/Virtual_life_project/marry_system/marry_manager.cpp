// Fill out your copyright notice in the Description page of Project Settings.


#include "marry_manager.h"
#include "Math/UnrealMathUtility.h"

void Umarry_manager::set_kid_custom_data(float _genetic_value, uint8 personality, FString input_text)
{
	genetic_value = _genetic_value;
	Personality = personality;
	hello_text = input_text;
}
