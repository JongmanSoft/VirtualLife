// Fill out your copyright notice in the Description page of Project Settings.


#include "Virtual_life_GameInstance.h"
#include "Sockets.h"
#include <Common/TcpSocketBuilder.h>
#include <Serialization/ArrayWriter.h>
#include "SocketSubsystem.h"

void UVirtual_life_GameInstance::ConnectServer()
{
	// 소켓 생성
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	// ip주소 넘겨주기.
	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port); // 내가 붙어야 하는 서버.

	// 디버깅용
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	// connected가 true이면 연결 성공.
	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected) { // 연결 성공
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// Session 생성
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run(); // Worker Thread 동작 시작

		// 1. Login Packet Send




		// 2. Login Result Packet Recv



	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}
