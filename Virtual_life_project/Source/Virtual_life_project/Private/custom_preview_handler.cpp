// Fill out your copyright notice in the Description page of Project Settings.


#include "custom_preview_handler.h"

// Sets default values for this component's properties
Ucustom_preview_handler::Ucustom_preview_handler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void Ucustom_preview_handler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void Ucustom_preview_handler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

