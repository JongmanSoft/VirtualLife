#pragma once

#include "Virtual_life_project/Virtual_life_project.h"
#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Sockets.h"
#include "Networking.h"

class FSocket;

class NetworkManager
{
};

class RecvManager : public FRunnable
{
public:
	RecvManager(FSocket* socket);
	~RecvManager();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy(); // 영원히 도는 것을 막기 위함

private:
	bool ReceivePacket(TArray<uint8>& OutPacket);
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool running = true;
	FSocket* Socket;
};