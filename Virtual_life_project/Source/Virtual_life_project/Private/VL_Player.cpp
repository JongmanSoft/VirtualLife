// Fill out your copyright notice in the Description page of Project Settings.


#include "VL_Player.h"
#include "AudioCaptureComponent.h"
#include "AudioDeviceManager.h"
#include "AudioMixerDevice.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "opus.h"

// Sets default values
AVL_Player::AVL_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->bRunPhysicsWithNoController = true;

}

AVL_Player::~AVL_Player()
{
}


// Called when the game starts or when spawned
void AVL_Player::BeginPlay()
{
	Super::BeginPlay();

	//인풋 컴포넌트 설정
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(FindComponentByClass<UInputComponent>());
	UInputMappingContext* MappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/VirtualLife_Character/Input/IMC_VirtualLife.IMC_VirtualLife"));
	if (MappingContext)
	{
		// PlayerController 가져오기
		APlayerController* PlayerController = GetController<APlayerController>();
		if (PlayerController)
		{
			// LocalPlayerSubsystem을 통해 매핑 컨텍스트 추가
			if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
				{
					Subsystem->AddMappingContext(MappingContext, 0); // 우선순위 0
				}
			}
		}
	}

	// Input Action Asset로 바인딩
	UInputAction* InteractAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/VirtualLife_Character/Input/IA_Door.IA_Door"));
	if (InteractAction)
	{
		Input->BindAction(InteractAction, ETriggerEvent::Completed, this, &AVL_Player::interact_action);
	}

	if (IsLocallyControlled())
	{
		// Opus 인코더 초기화
		int32 Error = 0;
		Encoder = opus_encoder_create(SampleRate, Channels, OPUS_APPLICATION_VOIP, &Error);
		if (Error != OPUS_OK)
		{
			UE_LOG(LogTemp, Error, TEXT("Opus encoder 생성 실패: %d"), Error);
			return;
		}

		// 오디오 캡처 설정
		DeviceParams.DeviceIndex = 0; // 기본 장치 사용

		uint32 NumFramesDesired = 960; // 예: 20ms @ 48kHz

		auto OnCapture = [this](const float* AudioData, int32 NumFrames, int32 NumChannels, int32 SampleRate, double StreamTime, bool bOverflow)
			{
				if (!Encoder || bOverflow || NumFrames == 0) return;

				// 1. float PCM → int16 PCM
				TArray<int16> PCM;
				PCM.SetNumUninitialized(NumFrames);
				for (int32 i = 0; i < NumFrames; ++i)
				{
					PCM[i] = FMath::Clamp(AudioData[i] * 32767.0f, -32768.f, 32767.f);
				}

				// 2. Opus 인코딩
				uint8 CompressedData[4000]; // 넉넉하게
				int32 CompressedBytes = opus_encode(
					Encoder,
					PCM.GetData(),
					NumFrames,            // frame_size: float으로 받은 수치 그대로
					CompressedData,
					sizeof(CompressedData)
				);

				if (CompressedBytes <= 0)
				{
					UE_LOG(LogTemp, Error, TEXT("Opus 인코딩 실패 (%d)"), CompressedBytes);
					return;
				}

				// 3. GameInstance 통해 서버로 전송
				if (GetWorld() && GetGameInstance()) {
					if (UGameInstance* GI = GetGameInstance())
					{
						if (auto* MyGI = Cast<UVirtual_life_GameInstance>(GI))
						{
							MyGI->SendVoicePacket(CompressedData, CompressedBytes);
							//UE_LOG(LogTemp, Log, TEXT("Opus %d bytes 전송됨"), CompressedBytes);
						}
					}
				}
			
			};

		bool bOpened = AudioCapture.OpenCaptureStream(DeviceParams, OnCapture, NumFramesDesired);
		if (bOpened)
		{
			AudioCapture.StartStream();
		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("오디오 캡처 스트림 열기 실패"));
		}
	}
}

void AVL_Player::CaptureVoiceFrame()
{
	//if (!Encoder) return;

	//float CapturedChannels = 0.0f;
	//float CapturedSampleRate = 0.0f;

	//Audio::FAlignedFloatBuffer PCMBuffer = MixerDevice->StopRecording(nullptr, CapturedChannels, CapturedSampleRate);

	//if (PCMBuffer.Num() == 0) return;

	//const float* PCMData = PCMBuffer.GetData();

	//// PCM(float) → int16 변환
	//TArray<int16> Int16PCM;
	//Int16PCM.SetNumUninitialized(PCMBuffer.Num());

	//for (int32 i = 0; i < PCMBuffer.Num(); ++i)
	//{
	//	Int16PCM[i] = FMath::Clamp<int32>(PCMData[i] * 32767.0f, -32768, 32767);
	//}

	//// Opus 인코딩
	//uint8 CompressedData[4000]; // 넉넉하게
	//int32 CompressedBytes = opus_encode(
	//	Encoder,
	//	Int16PCM.GetData(),
	//	OpusFrameSize, // 예: 960 (20ms @ 48kHz)
	//	CompressedData,
	//	sizeof(CompressedData)
	//);

	//if (CompressedBytes < 0)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Opus 인코딩 실패: %d"), CompressedBytes);
	//	return;
	//}

	//// GameInstance를 통해 전송
	//UGameInstance* GI = GetGameInstance();
	//if (GI)
	//{
	//	UVirtual_life_GameInstance* MyGI = Cast<UVirtual_life_GameInstance>(GI);
	//	if (MyGI)
	//	{
	//		MyGI->SendVoicePacket(CompressedData, CompressedBytes);
	//	}
	//}
}

// Called every frame
void AVL_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);




	// 여기서 목적지로 이동하고 회전하는 코드 작성

	if (false == isMyPlayer || false == myPlayer()) // 내가 조종하는 캐릭터가 아니라면
	{
		if (state == JUMP and GetCharacterMovement()->IsMovingOnGround()) // 점프 상태이고 바닥에 있으면
		{
			Jump();
		}
		else if (state == SEED)
		{
			USkeletalMeshComponent* SkeletalMeshComp = this->FindComponentByClass<USkeletalMeshComponent>();
			if (SkeletalMeshComp) {
				UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/fast_plant.fast_plant")), 1.0f);
				}
			}
		}
		else if (state == MINE)
		{
			USkeletalMeshComponent* SkeletalMeshComp = this->FindComponentByClass<USkeletalMeshComponent>();
			if (SkeletalMeshComp) {
				UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/fast_plant.fast_plant")), 1.0f);
				}
			}
		}
		else if (state == FISH)
		{
			USkeletalMeshComponent* SkeletalMeshComp = this->FindComponentByClass<USkeletalMeshComponent>();
			if (SkeletalMeshComp) {
				UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
				if (AnimInstance)
				{
					AnimInstance->Montage_Play(LoadObject<UAnimMontage>(nullptr, TEXT("/Game/animation/metahuman_fishing_Montage.metahuman_fishing_Montage")), 1.0f);
				}
			}
		}
		// todo: 애니메이션 동기화 해야 함.

		FVector TargetLocation(destInfo.x, destInfo.y, destInfo.z);
		FVector CurrentLocation = GetActorLocation();

		// 이동 방향 계산 (정규화)
		FVector Direction = (TargetLocation - CurrentLocation);
		Direction.Z = 0.f; // 수직 방향 제거
		float Distance = Direction.Size();

		if (Distance > 10.f) // 너무 가까우면 이동 안 해도 됨
		{
			Direction.Normalize();

			// 이동 입력 (이렇게 하면 애니메이션 블루프린트에도 반영됨)
			AddMovementInput(Direction, 1.0f);
		}

		// 회전 보간 처리
		FRotator TargetRotation(0.f, destInfo.yaw, 0.f);
		FRotator CurrentRotation = GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);

		SetActorRotation(NewRotation);
	}
}

// Called to bind functionality to input
void AVL_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVL_Player::Landed(const FHitResult& Hit)
{
	if ( true == myPlayer()) {
		Super::Landed(Hit);

		if (state == JUMP)
		{
			setMyState(IDLE); // 혹은 상황 따라 RUN으로도 가능
		}
	}
}

void AVL_Player::setCurInfo(PlayerInfo& v) // 현재 위치로 강제 이동
{
	curInfo = v;

	SetActorLocation(FVector(v.x, v.y, v.z));
	SetActorRotation(FRotator(0, v.yaw, 0));
}

void AVL_Player::setDestInfo(PlayerInfo& v) // 다음에 이동할 위치 설정
{
	// Dest에 최종 상태 복사.
	destInfo = v;
}

void AVL_Player::setMyState(int st)
{
	auto GameInstance = Cast<UVirtual_life_GameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance) { 
		std::lock_guard ll{ m };
		state = st;
		GameInstance->set_state(st);
		GameInstance->SendPlayerLocationToServer();
	}
}

void AVL_Player::setState(int st)
{
	state = st;
}

bool AVL_Player::myPlayer()
{
	UWorld* World = GetWorld();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			auto p = Cast<AVL_Player>(PlayerPawn);
			isMyPlayer = (this == p);
			if (false == isMyPlayer)
				int k = 0;
			return isMyPlayer;
		}
	}
	isMyPlayer = false;
	return isMyPlayer;
}

int32 AVL_Player::get_my_id()
{
	return m_id;
}

void AVL_Player::set_my_id(const unsigned int& new_id)
{
	m_id = new_id;
}

void AVL_Player::interact_action()
{
	UE_LOG(LogTemp, Log, TEXT("DOOR키누름"));
}

