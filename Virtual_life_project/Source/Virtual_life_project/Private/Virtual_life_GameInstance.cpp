// Fill out your copyright notice in the Description page of Project Settings.


#include "Virtual_life_GameInstance.h"
#include "Sockets.h"
#include <Common/TcpSocketBuilder.h>
#include <Serialization/ArrayWriter.h>
#include "SocketSubsystem.h"

void UVirtual_life_GameInstance::ConnectServer()
{
	// ���� ����
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	// ip�ּ� �Ѱ��ֱ�.
	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port); // ���� �پ�� �ϴ� ����.

	// ������
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	// connected�� true�̸� ���� ����.
	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected) { // ���� ����
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// Session ����
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run(); // Worker Thread ���� ����

		// 1. Login Packet Send




		// 2. Login Result Packet Recv



	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}
