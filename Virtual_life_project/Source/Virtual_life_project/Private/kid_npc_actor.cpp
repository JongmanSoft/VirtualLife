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

void Akid_npc_actor::find_hair_groom()
{
    
    TArray<UActorComponent*> Components;
    GetComponents(Components);
    for (UActorComponent* Component : Components)
    {
        if (UGroomComponent* GroomComp = Cast<UGroomComponent>(Component))
        {
            if (GroomComp->ComponentHasTag(FName("Hair")))
            {
                m_hair_groom = GroomComp;
            }
        }
    }
}

void Akid_npc_actor::set_groom()
{
    //머리 
    //TODO : 머리 하나더 크리에이터에서 갖고오자
    if (m_hair_groom) {
        TCHAR groom_asset_file[3][100]
            = { TEXT("/Game/MetaHumans/real_kid/MaleHair_fro/Hair/Hair_S_SideSweptFringe.Hair_S_SideSweptFringe")
            ,TEXT("/Game/MetaHumans/real_kid/FemaleHair_pfn/Hair/Hair_S_UpdoBraids.Hair_S_UpdoBraids"),
            TEXT("/Game/MetaHumans/naked_character/MaleHair_fro/Hair/Hair_S_Casual.Hair_S_Casual")
        };
        TCHAR groom_binding_asset_file[3][150]
            = { TEXT("/Game/MetaHumans/real_kid/MaleHair_fro/GroomBinding/Hair_S_SideSweptFringe_Binding.Hair_S_SideSweptFringe_Binding")
            ,TEXT("/Game/MetaHumans/real_kid/FemaleHair_pfn/GroomBinding/Hair_S_UpdoBraids_Binding.Hair_S_UpdoBraids_Binding")
            ,TEXT("/Game/MetaHumans/naked_character/MaleHair_fro/GroomBinding/Hair_S_Casual_Binding.Hair_S_Casual_Binding")
        };

        auto a = LoadObject<UGroomAsset>(nullptr, groom_asset_file[m_custom->hair]);
        m_hair_groom->SetGroomAsset(a);
        m_hair_groom->SetBindingAsset(LoadObject<UGroomBindingAsset>(nullptr, groom_binding_asset_file[m_custom->hair]));
    }

   //눈썹
    //todo:눈썹 2개 메타휴먼 크리에이터에서 갖고올것
    UGroomComponent* owner_brows = Cast<UGroomComponent>(FindComponentByTag(UGroomComponent::StaticClass(), FName("EYEBROW")));
    if (owner_brows) {
        TCHAR groom_asset_file[3][100]
            = { TEXT("/Game/MetaHumans/real_kid/FemaleHair/Hair/Eyebrows_S_Shaded.Eyebrows_S_Shaded")
            ,TEXT("/Game/MetaHumans/naked_character/FemaleHair/Hair/Eyebrows_L_Shaded.Eyebrows_L_Shaded"),
            TEXT("/Game/MetaHumans/gyungcheal_id/MaleHair/Hair/Eyebrows_S_FlatThin.Eyebrows_S_FlatThin")
        };
        TCHAR groom_binding_asset_file[3][150]
            = { TEXT("/Game/MetaHumans/real_kid/FemaleHair/GroomBinding/Eyebrows_S_Shaded_Binding.Eyebrows_S_Shaded_Binding")
            ,TEXT("/Game/MetaHumans/naked_character/FemaleHair/GroomBinding/Eyebrows_L_Shaded_Binding.Eyebrows_L_Shaded_Binding")
            ,TEXT("/Game/MetaHumans/gyungcheal_id/MaleHair/GroomBinding/Eyebrows_S_FlatThin_Binding.Eyebrows_S_FlatThin_Binding")
        };


        auto a = LoadObject<UGroomAsset>(nullptr, groom_asset_file[m_custom->eyebrows]);
        owner_brows->SetGroomAsset(a);
        owner_brows->SetBindingAsset(LoadObject<UGroomBindingAsset>(nullptr, groom_binding_asset_file[m_custom->eyebrows]));
    }
}

void Akid_npc_actor::set_skeltal()
{
    //상의
    TCHAR torso_object_path[2][150] = {TEXT("/Game/MetaHumans/Common/Male/Short/UnderWeight/Tops/Crewneckt/m_srt_unw_top_crewneckt_nrm_Medium.m_srt_unw_top_crewneckt_nrm_Medium"),
        TEXT("/Game/MetaHumans/Common/Male/Short/UnderWeight/Tops/Sweater/m_srt_unw_top_sweater_nrm_Medium.m_srt_unw_top_sweater_nrm_Medium") };

    USkeletalMeshComponent* torso_skeltal = find_tag_skeltal_mesh(FName("Torso"));
    if (torso_skeltal) {
        torso_skeltal->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, torso_object_path[m_custom->shirt]));
    }

    //하의
    TCHAR pants_object_path[2][150] = {
        TEXT("/Game/MetaHumans/Common/Male/Short/UnderWeight/Bottoms/Shorts/m_srt_unw_btm_shorts_nrm_Medium.m_srt_unw_btm_shorts_nrm_Medium"),
        TEXT("/Game/MetaHumans/Common/Male/Short/UnderWeight/Bottoms/Jeans/m_srt_unw_btm_jeans_nrm_Medium.m_srt_unw_btm_jeans_nrm_Medium")
    };
    USkeletalMeshComponent* pants_skeltal = find_tag_skeltal_mesh(FName("Pants"));
    if (pants_skeltal) {
        pants_skeltal->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, pants_object_path[m_custom->pants]));
    }

	//신발
    TCHAR shoes_object_path[2][150] = {
      TEXT("/Game/MetaHumans/Common/Male/Short/UnderWeight/Shoes/Oxfords/m_srt_unw_shs_oxfords_Medium.m_srt_unw_shs_oxfords_Medium"),
      TEXT("/Game/MetaHumans/Common/Male/Short/UnderWeight/Shoes/Flipflops/m_srt_unw_shs_flipflops_Medium.m_srt_unw_shs_flipflops_Medium")
    };
    USkeletalMeshComponent* shoes_skeltal = find_tag_skeltal_mesh(FName("Shoes"));
    if (shoes_skeltal) {
        shoes_skeltal->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, shoes_object_path[m_custom->pants]));
    }

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
            BodyMaterial->SetScalarParameterValue(FName("BaseColor_Brightness"), m_custom->skin);
     
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
            FACEMaterial->SetScalarParameterValue(FName("BaseColor_Brightness"), m_custom->skin);
        }
    }
    //눈 머터리얼
    UMaterialInterface* EYE_material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/MyMaterial.MyMaterial"));
    if (EYE_material)
    {
        if (face_mesh)
        {
            UMaterialInstanceDynamic* LEFT_EYE_Material = UMaterialInstanceDynamic::Create(EYE_material, this);
            face_mesh->SetMaterial(1, LEFT_EYE_Material);
            UMaterialInstanceDynamic* RIGHT_EYE_Material = UMaterialInstanceDynamic::Create(EYE_material, this);
            face_mesh->SetMaterial(2, RIGHT_EYE_Material);


            LEFT_EYE_Material->SetScalarParameterValue(FName("Iris_Diameter"), m_custom->eye_scale);
            LEFT_EYE_Material->SetScalarParameterValue(FName("PupilScale"), m_custom->pupil_scale);
            LEFT_EYE_Material->SetScalarParameterValue(FName("IrisColor2U"), m_custom->L_eye_color_hue);
            LEFT_EYE_Material->SetScalarParameterValue(FName("IrisColor1V"), m_custom->L_eye_color_sat);

           RIGHT_EYE_Material->SetScalarParameterValue(FName("Iris_Diameter"), m_custom->eye_scale);
           RIGHT_EYE_Material->SetScalarParameterValue(FName("PupilScale"), m_custom->pupil_scale);
           RIGHT_EYE_Material->SetScalarParameterValue(FName("IrisColor2U"), m_custom->R_eye_color_hue);
           RIGHT_EYE_Material->SetScalarParameterValue(FName("IrisColor1V"), m_custom->R_eye_color_sat);
        }
    }
 
	//머리 머터리얼
    if (m_hair_groom) {
        UMaterialInterface* Hair_material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/MetaHumans/woman/Materials/MI_Hair_Cards.MI_Hair_Cards"));
        if (Hair_material) {
            UMaterialInstanceDynamic* Hair_instance = UMaterialInstanceDynamic::Create(Hair_material, this);
            m_hair_groom->SetMaterial(0, Hair_instance);
            m_hair_groom->SetMaterial(1, Hair_instance);
            m_hair_groom->SetMaterial(2, Hair_instance);
            Hair_instance->SetScalarParameterValue(FName("hair_R"), m_custom->hair_color_R);
            Hair_instance->SetScalarParameterValue(FName("hair_G"), m_custom->hair_color_G);
            Hair_instance->SetScalarParameterValue(FName("hair_B"), m_custom->hair_color_B);
        }
    }


}

void Akid_npc_actor::set_morph_target()
{
    //todo: 언리얼에디터에서 모프타겟만들자..;
    USkeletalMeshComponent* face_mesh = find_tag_skeltal_mesh(FName("Face"));
    face_mesh->SetMorphTarget(FName("eye_slope"), m_custom-> eye_slope  , false);
    face_mesh->SetMorphTarget(FName("eye_width"), m_custom-> eye_width  , false);
    face_mesh->SetMorphTarget(FName("eye_thick"), m_custom-> eye_thick  , false);
    face_mesh->SetMorphTarget(FName("nose_width"), m_custom-> nose_width , false);
    face_mesh->SetMorphTarget(FName("nose_height"), m_custom-> nose_height,false);
    face_mesh->SetMorphTarget(FName("mouse_width"), m_custom-> mouse_width,false);
    face_mesh->SetMorphTarget(FName("mouse_thick"), m_custom-> mouse_thick,false);
    face_mesh->SetMorphTarget(FName("mouse_slope"), m_custom-> mouse_slope,false);
    face_mesh->SetMorphTarget(FName("face_width"), m_custom->face_width, false);
    face_mesh->SetMorphTarget(FName("heavy"), m_custom->heavy, false);
    face_mesh->SetMorphTarget(FName("chin"), m_custom->chin, false);
    face_mesh->SetMorphTarget(FName("jaw"), m_custom->jaw, false);

}

void Akid_npc_actor::apply_custom()
{
    set_groom();
    set_skeltal();
    create_dynamic_mat_custom();
    set_morph_target();
}

