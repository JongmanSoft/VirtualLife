// Fill out your copyright notice in the Description page of Project Settings.


#include "VL_Player.h"

// Sets default values
AVL_Player::AVL_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
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

	if (false == myPlayer()) // 내가 조종하는 캐릭터가 아니라면
	{
		if (true || curInfo.st == RUN) // 이동중이라면
		{
			/*SetActorRotation(FRotator(0, destInfo.yaw, 0));
			SetActorLocation(FVector(destInfo.x, destInfo.y, destInfo.z));*/
			// 목표 위치와 현재 위치를 보간하여 천천히 이동
			FVector TargetLocation = FVector(destInfo.x, destInfo.y, destInfo.z);
			FVector CurrentLocation = GetActorLocation();

			// VInterpTo 함수를 사용하여 서서히 이동
			FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 5.0f); // 5.0f는 이동 속도 조절

			// 새로운 위치로 이동
			SetActorLocation(NewLocation);

			// 회전도 천천히 보간하여 회전
			FRotator TargetRotation(0.f, destInfo.yaw, 0.f);
			FRotator CurrentRotation = GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f); // 5.0f는 회전 속도

			// 새로운 회전으로 설정
			SetActorRotation(NewRotation);
		}
	}
}

// Called to bind functionality to input
void AVL_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
			return this == p;
		}
	}
	return false;
}

