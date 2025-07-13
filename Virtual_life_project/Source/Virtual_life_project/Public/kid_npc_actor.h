// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Custom/Custom_data.h"
#include "kid_npc_actor.generated.h"

UCLASS()
class VIRTUAL_LIFE_PROJECT_API Akid_npc_actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Akid_npc_actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//д©╫╨ер©К
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* MI_hair = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClass")
	UCustom_data* m_custom;
};
