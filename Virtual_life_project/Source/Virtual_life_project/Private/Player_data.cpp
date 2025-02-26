// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_data.h"

UPlayer_data::UPlayer_data()
{
	player_name = "";
	player_job = static_cast<Job>(0); // 직업값에 냅다 정수넣을라면 이지랄까지해야댐..,,흠 enum쓰는게맞나;; 일단씀
	age = 0;
	hp = 100;
	mp = 100;

}

void UPlayer_data::set_MP_s(int new_mp)
{
	if (new_mp > 100) mp = 100;
	else if (new_mp < 0) mp = 0;
	else hp = new_mp;

}

void UPlayer_data::set_HP_s(int new_hp)
{
	if (new_hp > 100) hp = 100;
	else if (new_hp < 0) hp = 0;
	else hp = new_hp;

}
