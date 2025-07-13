// Fill out your copyright notice in the Description page of Project Settings.


#include "kid_npc_actor.h"

// Sets default values
Akid_npc_actor::Akid_npc_actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Akid_npc_actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Akid_npc_actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

