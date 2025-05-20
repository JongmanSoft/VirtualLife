// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Virtual_life_GameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AudioCaptureComponent.h"
#include "AudioDeviceManager.h"
#include "AudioMixerDevice.h"
#include "VL_Player.generated.h"

UCLASS()
class VIRTUAL_LIFE_PROJECT_API AVL_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVL_Player();
	virtual ~AVL_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CaptureVoiceFrame();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override; // ← 여기!!

	void setCurInfo(PlayerInfo& v); // 현재위치 강제 이동하는 함수
	void setDestInfo(PlayerInfo& v);

	UFUNCTION(BlueprintCallable)
	void setMyState(int st); // 3번이 점프임 명심하도록
	
	void setState(int st);

	bool isMyPlayer = false;

	bool myPlayer();

	UFUNCTION(BlueprintCallable)
	int32 get_my_id();

	void set_my_id(const unsigned int& new_id);
protected:
	unsigned int m_id;
	PlayerInfo curInfo; // 현재 위치
	PlayerInfo destInfo; // 목적지
	int state;
private:
	std::mutex m;

	FTimerHandle VoiceCaptureTimer;

	UPROPERTY()
	UAudioCaptureComponent* MicCapture = nullptr;
	USoundSubmix* Submix = nullptr;
	Audio::FMixerDevice* MixerDevice = nullptr;


	OpusEncoder* Encoder = nullptr;
	int32 OpusFrameSize = 960; // 20ms @ 48kHz
	int32 SampleRate = 48000;
	int32 Channels = 1;
};
