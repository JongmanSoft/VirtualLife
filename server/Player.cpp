#include "stdafx.h"
#include "Player.h"

void Player::send(void* packet)
{
	EXT_OVER* ov = new EXT_OVER();

	// 패킷 크기 복사
	unsigned short p_size;
	memcpy(&p_size, packet, 2);
	ov->setup_send(reinterpret_cast<char*>(packet), p_size);

	int result = WSASend(socket, &ov->wsabuf, 1, 0, 0, &ov->over, NULL);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			printf("WSASend failed with error: %d\n", error);
			closesocket(socket);
		}
	}
}

void Player::recv()
{
	if (socket == INVALID_SOCKET) {
		printf("Invalid socket\n");
		return;
	}

	ZeroMemory(&over.over, sizeof(over.over));
	over.wsabuf.len = BUFSIZE - packet_data.size();
	over.wsabuf.buf = over.wb_buf + packet_data.size();

	if (over.wsabuf.buf == nullptr) {
		printf("Buffer is null\n");
		return;
	}

	DWORD flags = 0;
	DWORD bytesReceived = 0;

	int result = WSARecv(socket, &over.wsabuf, 1, &bytesReceived, &flags, &over.over, NULL);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			printf("WSARecv failed with error: %d\n", error);
			closesocket(socket);
			WSACleanup();
		}
	}
}

void Player::handle_packet(char* packet, unsigned short length)
{
    char type = packet[2];

    switch (type)
    {
    case CS_LOGIN:// ok
    {
        CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
        break;
    }
    case CS_CHAT:
    {
        CS_CHAT_PACKET* p = reinterpret_cast<CS_CHAT_PACKET*>(packet);
        break;
    }
    case CS_LEAVE:
    {
        break;
    }
    default:

        break;
    }
}
