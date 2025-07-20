// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Kid_custom_ui.h"
#include "VL_Player.h"
#include "../Public/Virtual_life_GameInstance.h"
#include "kid_map_script.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUAL_LIFE_PROJECT_API Akid_map_script : public ALevelScriptActor
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    UUserWidget* WidgetInstance;

	UFUNCTION()
	void custom_finish(float g_value, uint8 per_value, FString hello, FString name);
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ACharacter* you_character; //상대캐릭터

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* preview_kid; //아이

	void set_customizing_inteerpol(Customizing& change_cus, const Customizing& standard_cus, float interpol_value);
	Customizing make_kid_customizing(const Customizing& my_custom, const Customizing& you_custom);
	void make_finish_widget(const FString& kid_name, const int& personality, const FString& hello_text);
	void kid_set_animation(int personality);
};
