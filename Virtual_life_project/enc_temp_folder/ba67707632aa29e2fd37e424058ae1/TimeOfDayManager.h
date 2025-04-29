// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "TimeOfDayManager.generated.h"

class AVirtualLifeController;
class UGamePlayMainWidget;
class UClockWidget;

UCLASS()
class VIRTUAL_LIFE_PROJECT_API ATimeOfDayManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATimeOfDayManager();

protected:
	virtual void BeginPlay() override;

	// Tick용 타이머
	FTimerHandle TimerHandle;

	// Sun / Moon Light
	UPROPERTY(EditAnywhere, Category = "Lighting")
	ADirectionalLight* SunLight;

	UPROPERTY(EditAnywhere, Category = "Lighting")
	ADirectionalLight* MoonLight;

	// 시간 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float Time = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeStep = 0.1f;

	// 현재 낮인지 밤인지
	bool bIsDaytime = true;

	void IncrementTime();
	void UpdateSun();
	void NotifyListeners(bool bDaytime);

	// Sunrise/Sunset 처리
	void OnSunrise();
	void OnSunset();

	UFUNCTION(BlueprintCallable)
	void StartTimer();
};
