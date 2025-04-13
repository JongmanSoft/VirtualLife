// Fill out your copyright notice in the Description page of Project Settings.


#include "VL_Player.h"

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

