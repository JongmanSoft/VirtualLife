#pragma once

#include "Virtual_life_project/Virtual_life_project.h"
#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Sockets.h"
#include "Networking.h"
#include "Virtual_life_GameInstance.h"

class UVirtual_life_GameInstance;

class RecvManager : public FRunnable
{
public:
	RecvManager(FSocket* socket, UVirtual_life_GameInstance* ins);
	~RecvManager();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy(); // ������ ���� ���� ���� ����

private:
	bool ReceivePacket();
	bool ProcessPacket();
	TArray<uint8> Buffer;

protected:
	FRunnableThread* Thread = nullptr;
	bool running = true;
	FSocket* Socket;
	UVirtual_life_GameInstance* Instance;
};

class SendWorker : public FRunnable // �𸮾� �������� �����带 ����ϴ� ��
{
public:
	SendWorker(FSocket* socket, UVirtual_life_GameInstance* ins);
	~SendWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy(); // ������ ���� ���� ���� ����

	bool SendPacket(void* packet, int32 PacketSize);

private:
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket;
	UVirtual_life_GameInstance* Instance;
};