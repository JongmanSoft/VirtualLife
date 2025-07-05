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
#include "../interface/CPI_interface.h"


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

	UInputMappingContext* MappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/VirtualLife_Character/Input/IMC_VirtualLife.IMC_VirtualLife"));
	if (MappingContext)
	{
		if (APlayerController* PlayerController = GetController<APlayerController>())
		{
			if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
				{
					Subsystem->AddMappingContext(MappingContext, 0);
				}
			}
		}
	}
	
	// 사운드웨이브 및 오디오 컴포넌트 생성
	ProceduralSoundWave = NewObject<USoundWaveProcedural>(this);
	ProceduralSoundWave->SetSampleRate(SampleRate);
	ProceduralSoundWave->NumChannels = 1;
	ProceduralSoundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	ProceduralSoundWave->bLooping = false;
	ProceduralSoundWave->bCanProcessAsync = true;
	ProceduralSoundWave->SoundGroup = SOUNDGROUP_Voice;
	ProceduralSoundWave->bProcedural = true;
	ProceduralSoundWave->Pitch = 0.85f;
	ProceduralSoundWave->Volume = 5.f;

	AudioComponent = NewObject<UAudioComponent>(this);
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetSound(ProceduralSoundWave);
	AudioComponent->bAllowSpatialization = false;
	AudioComponent->AttenuationSettings = nullptr;

	AudioComponent->RegisterComponent();

	if (!AudioComponent->IsPlaying())
	{
		AudioComponent->Play();
	}


	// Opus 디코더 초기화
	int32 Error = 0;
	Decoder = opus_decoder_create(SampleRate, 1, &Error);
	if (Error != OPUS_OK)
	{
		UE_LOG(LogTemp, Error, TEXT("Opus 디코더 생성 실패: %d"), Error);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Opus 디코더 생성 성공"));
	}


	if (IsLocallyControlled()) // 조작중인 플레이어에게만
	{
		// Opus 인코더 초기화
		Error = 0;
		Encoder = opus_encoder_create(SampleRate, 1, OPUS_APPLICATION_VOIP, &Error);
		if (Error != OPUS_OK)
		{
			UE_LOG(LogTemp, Error, TEXT("Opus encoder 생성 실패: %d"), Error);
			return;
		}
		opus_encoder_ctl(Encoder, OPUS_SET_BITRATE(96000)); // 또는 96000
		opus_encoder_ctl(Encoder, OPUS_SET_COMPLEXITY(10)); // 최대 품질
		opus_encoder_ctl(Encoder, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE)); // 음성에 최적화

		// 오디오 캡처 설정
		DeviceParams.DeviceIndex = 0; // 기본 장치 사용

		// 오디오 캡쳐 콜백 함수
		OnCapture = [this](const float* AudioData, int32 NumFrames, int32 NumChannels, int32 SampleRate, double StreamTime, bool bOverflow)
			{
				if (!Encoder || bOverflow || NumFrames == 0) return;

				constexpr int32 FrameSize = 960;
				constexpr float Gain = 5.0f;

				// 1. AudioData 누적
				AudioBuffer.Append(AudioData, NumFrames);

				// 2. 버퍼가 960개 이상 쌓이면 처리
				while (AudioBuffer.Num() >= FrameSize)
				{
					{
						//// 2-1. float → int16 변환 (with gain)
						//TArray<int16> PCM;
						//PCM.SetNumUninitialized(FrameSize);
						//for (int32 i = 0; i < FrameSize; ++i)
						//{
						//	float boosted = AudioBuffer[i] * Gain;
						//	PCM[i] = FMath::Clamp(boosted * 32767.0f, -32768.f, 32767.f);
						//}

						//// 2-2. Opus 인코딩
						//uint8 CompressedData[4000];
						//int32 CompressedBytes = opus_encode(
						//	Encoder,
						//	PCM.GetData(),
						//	FrameSize,
						//	CompressedData,
						//	sizeof(CompressedData)
						//);

						//if (CompressedBytes <= 0)
						//{
						//	UE_LOG(LogTemp, Error, TEXT("Opus 인코딩 실패 (%d)"), CompressedBytes);
						//	break; // 나머지 처리 중단
						//}

						//// 2-3. 디코딩
						//int16 DecodedPCM[1920];
						//int32 DecodedSamples = opus_decode(
						//	Decoder,
						//	CompressedData,
						//	CompressedBytes,
						//	DecodedPCM,
						//	FrameSize,
						//	0
						//);

						//if (DecodedSamples <= 0)
						//{
						//	UE_LOG(LogTemp, Error, TEXT("Opus 디코딩 실패 (%d)"), DecodedSamples);
						//	break;
						//}

						//// 2-4. 재생
						//if (ProceduralSoundWave)
						//{
						//	ProceduralSoundWave->QueueAudio((uint8*)DecodedPCM, DecodedSamples * sizeof(int16));
						//}
					}

					// 인코딩 안하는 버전
					TArray<int16> PCM;
					PCM.SetNumUninitialized(FrameSize);
					for (int32 i = 0; i < FrameSize; ++i)
					{
						float boosted = AudioBuffer[i] * Gain;
						PCM[i] =  FMath::Clamp(boosted * 32767.0f, -32768.f, 32767.f);
					}

					if (ProceduralSoundWave)
					{
						ProceduralSoundWave->QueueAudio((uint8*)PCM.GetData(), FrameSize * sizeof(int16));
					}

					UE_LOG(LogTemp, Log, TEXT("Mic Capture Rate: %d Hz, Channels: %d"), SampleRate, NumChannels);

					// 2-5. 누적 버퍼에서 사용한 샘플 제거
					AudioBuffer.RemoveAt(0, FrameSize, /*bAllowShrinking*/ false);
				}
			};
	}
}

void AVL_Player::CaptureVoiceFrame()
{
	
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

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UInputAction* InteractAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/VirtualLife_Character/Input/IA_Door.IA_Door"));
		if (InteractAction)
		{
			Input->BindAction(InteractAction, ETriggerEvent::Completed, this, &AVL_Player::interact_action);
		}
	}
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

void AVL_Player::Start_Audio_Capture()
{
	if (!AudioCapture.IsStreamOpen())
	{
		bool bOpened = AudioCapture.OpenCaptureStream(DeviceParams, OnCapture, 960);
		if (!bOpened)
		{
			return;
		}
	}

	if (!AudioCapture.IsCapturing())
	{
		AudioCapture.StartStream();
	}
}

void AVL_Player::Stop_Audio_Capture()
{
	if (AudioCapture.IsCapturing())
	{
		AudioCapture.StopStream();
	}

	if (AudioCapture.IsStreamOpen())
	{
		AudioCapture.CloseStream(); 
	}
}

void AVL_Player::set_my_id(const unsigned int& new_id)
{
	m_id = new_id;
}

void AVL_Player::interact_action()
{
	UE_LOG(LogTemp, Log, TEXT("Door KEY compressed"));
	TArray<AActor*> OverlappingActors;
	cap->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != this) // 자기 자신 제외
		{
			if (Actor->GetClass()->ImplementsInterface(UCPI_interface::StaticClass()))ICPI_interface::Execute_Interact(Actor);
		}
	}
}

void AVL_Player::EndPlay(const EEndPlayReason::Type EndPlayReason) // 종료시 오디오캡쳐 끄는 것
{
	if (AudioCapture.IsStreamOpen())
	{
		AudioCapture.StopStream();
		AudioCapture.CloseStream();
	}

	if (Encoder)
	{
		opus_encoder_destroy(Encoder);
		Encoder = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void AVL_Player::HandleVoicePacket(const SC_VOICE_CHAT_PACKET& p)
{
	if (!Decoder || !AudioComponent || !ProceduralSoundWave) return;

	int16 DecodedPCM[960 * 2];
	int FrameSize = opus_decode(
		Decoder,
		reinterpret_cast<const unsigned char*>(p.data),
		p.data_len,
		DecodedPCM,
		960,
		0
	);
	if (FrameSize <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Opus 디코딩 실패: %d"), FrameSize);
		return;
	}

	ProceduralSoundWave->QueueAudio(reinterpret_cast<uint8*>(DecodedPCM), FrameSize * sizeof(int16));

	if (!AudioComponent->IsPlaying())
	{
		AudioComponent->Play();
	}
}