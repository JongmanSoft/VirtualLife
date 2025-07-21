// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroomComponent.h"
#include "../Custom/Custom_data.h"
#include "../../server/protocol.h"
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

	void set_new_custom(Customizing recv_cus);
	void set_kid_info(const FString& name, const FString& hello, const int& personality);
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString name; //이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString hello;//인사말
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int personality; //성격

public:
	USkeletalMeshComponent* find_tag_skeltal_mesh(const FName& tag_name);
	void find_hair_groom();


	void set_groom(); // 머리변경
	void set_skeltal(); // 옷변경
	void create_dynamic_mat_custom();
	void set_morph_target();
	void apply_custom();
public:
	//커스텀용
	UGroomComponent* m_hair_groom = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* MI_hair = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomClass")
	UCustom_data* m_custom;


};
