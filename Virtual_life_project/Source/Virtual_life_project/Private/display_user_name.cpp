// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TextBlock.h"
#include "display_user_name.h"

void Udisplay_user_name::set_name(const FText& new_name)
{
	user_name_text->SetText(new_name);
}
