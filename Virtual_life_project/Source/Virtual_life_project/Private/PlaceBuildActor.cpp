// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceBuildActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APlaceBuildActor::APlaceBuildActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void APlaceBuildActor::SetMesh(UStaticMesh* StaticMesh)
{
    if (StaticMesh)
    {
        Mesh->SetStaticMesh(StaticMesh);
    }
}
