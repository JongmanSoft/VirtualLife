// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Player_data.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class Job : uint8
{
	farmer,    // ³óºÎ
	fishermen, // ¾îºÎ
	miners  // ±¤ºÎ
};

UCLASS()
class VIRTUAL_LIFE_PROJECT_API UPlayer_data : public UObject
{
	GENERATED_BODY()

public:
	UPlayer_data();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profile")
	FString player_name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profile")
	Job player_job;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Profile")
	uint8 age;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "healthy")
	int32 hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "healthy")
	int32 mp;

public:
	void set_HP_s(int new_hp);
	void set_MP_s(int new_mp);


};
