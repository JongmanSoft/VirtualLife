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

    apply_custom();

}

// Called every frame
void Akid_npc_actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USkeletalMeshComponent* Akid_npc_actor::find_tag_skeltal_mesh(const FName& tag_name)
{
    TArray<UActorComponent*> Components;
    GetComponents(Components);
    for (UActorComponent* Component : Components)
    {
        if (USkeletalMeshComponent* SkeletalComp = Cast<USkeletalMeshComponent>(Component))
        {
            if (SkeletalComp->ComponentHasTag(tag_name))
            {
                return SkeletalComp;
            }
        }
    }
    return nullptr;
}

void Akid_npc_actor::set_groom()
{
}

void Akid_npc_actor::set_skeltal()
{
}

void Akid_npc_actor::create_dynamic_mat_custom()
{
    //몸 피부 머터리얼
    UMaterialInterface* Body_material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/MyMaterial.MyMaterial"));
    USkeletalMeshComponent* body_mesh = find_tag_skeltal_mesh(FName("Face"));
    if (Body_material)
    {
        if (body_mesh)
        {
            UMaterialInstanceDynamic* BodyMaterial = UMaterialInstanceDynamic::Create(Body_material, this);
            body_mesh->SetMaterial(0, BodyMaterial);
     
        }
    }


	//페이스 피부 머터리얼 
    UMaterialInterface* Face_material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/MyMaterial.MyMaterial"));
    USkeletalMeshComponent* face_mesh = find_tag_skeltal_mesh(FName("Face"));
    if (Face_material)
    {
        if (face_mesh)
        {
            UMaterialInstanceDynamic* FACEMaterial = UMaterialInstanceDynamic::Create(Face_material, this);
            face_mesh->SetMaterial(6, FACEMaterial);
            face_mesh->SetMaterial(7, FACEMaterial);
        }
    }
    //왼쪽눈 머터리얼 
    UMaterialInterface* EYE_material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/MyMaterial.MyMaterial"));
    if (Face_material)
    {
        if (face_mesh)
        {
            UMaterialInstanceDynamic* LEFT_EYE_Material = UMaterialInstanceDynamic::Create(Face_material, this);
            face_mesh->SetMaterial(1, LEFT_EYE_Material);
            UMaterialInstanceDynamic* RIGHT_EYE_Material = UMaterialInstanceDynamic::Create(Face_material, this);
            face_mesh->SetMaterial(2, RIGHT_EYE_Material);
        }
    }
 

}

void Akid_npc_actor::set_morph_target()
{
}

void Akid_npc_actor::apply_custom()
{
    set_groom();
    set_skeltal();
    create_dynamic_mat_custom();
    set_morph_target();
}

