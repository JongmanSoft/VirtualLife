// Fill out your copyright notice in the Description page of Project Settings.


#include "LogoDisplayGameMode.h"
#include "Kismet/GameplayStatics.h"

void ALogoDisplayGameMode::MoveToLoginMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("LogInMap"));
}
