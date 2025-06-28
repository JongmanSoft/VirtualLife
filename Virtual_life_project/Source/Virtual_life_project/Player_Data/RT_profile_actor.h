// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Custom/m_CustomizableSkeletalComponent.h"
#include "RT_profile_actor.generated.h"


UCLASS()
class VIRTUAL_LIFE_PROJECT_API ART_profile_actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ART_profile_actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//Ä¸Ã³ ÄÄÆ÷³ÍÆ® 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profile")
    USceneCaptureComponent2D* SceneCaptureComponent;

	//ÀÓ½Ã ½ºÄÌ·¹Å»¸Þ½Ã,Ä¿½ºÅÒ Á¤º¸
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profile")
	Um_CustomizableSkeletalComponent* Custom_skeletal_component;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profile")
	USkeletalMesh* skeletal_component;
	
	UFUNCTION(BlueprintCallable)
	void init_my_character();

	UFUNCTION(BlueprintCallable)
	void init_other_character(uint8& id);
};
