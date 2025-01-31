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

	void Destroy(); // 영원히 도는 것을 막기 위함

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

class SendWorker : public FRunnable // 언리얼 엔진에서 스레드를 사용하는 법
{
public:
	SendWorker(FSocket* socket, UVirtual_life_GameInstance* ins);
	~SendWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy(); // 영원히 도는 것을 막기 위함

	bool SendPacket(void* packet, int32 PacketSize);

private:
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket;
	UVirtual_life_GameInstance* Instance;
};