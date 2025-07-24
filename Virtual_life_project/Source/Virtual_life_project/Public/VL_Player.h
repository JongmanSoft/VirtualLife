// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Virtual_life_GameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void setCurInfo(PlayerInfo& v); // 현재위치 강제 이동하는 함수
	void setDestInfo(PlayerInfo& v);

	UFUNCTION(BlueprintCallable)
	void setMyState(int st); // 3번이 점프임 명심하도록
	
	void setState(int st);

	void setFeel(int feel);

	
	bool isMyPlayer = false;

	UFUNCTION(BlueprintCallable)
	bool myPlayer();

	UFUNCTION(BlueprintCallable)
	int32 get_my_id();

	void set_my_id(const unsigned int& new_id);


	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	unsigned int m_id;
	PlayerInfo curInfo; // 현재 위치
	PlayerInfo destInfo; // 목적지
	int state;
	int prev_state;
	int feel;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* cap;//의존성주입해야함

	void interact_action();

	void not_move(); //목적지를 현재위치로 설정

	UFUNCTION(BlueprintCallable)
	void player_action(int action);

	UFUNCTION(BlueprintCallable)
	void player_feel(int feel_state);
private:
	std::mutex m;
public:

	float MoveTimeout = 3.0f; // 3초
	float MoveElapsed = 0.0f;
	bool bIsMovingToTarget = false;
};
