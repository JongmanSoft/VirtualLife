// Fill out your copyright notice in the Description page of Project Settings.


#include "VL_Player.h"

// Sets default values
AVL_Player::AVL_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AVL_Player::~AVL_Player()
{
}

// Called when the game starts or when spawned
void AVL_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVL_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVL_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

