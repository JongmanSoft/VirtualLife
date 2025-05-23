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
	float ServerTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float ClientTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeStep = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float  LocalReceiveTime = 0.0f;

	bool bIsDaytime = true;

	void IncrementTime();
	void SeverUpdateSun();
	void ClientUpdateSun();
	void NotifyListeners(bool bDaytime);

	void OnSunrise();
	void OnSunset();

	UFUNCTION(BlueprintCallable)
	void StartTimer();

	UPROPERTY(BlueprintReadWrite)
	bool bIsTimerRunning = true;
};
