#include "stdafx.h"
#include "Player.h"

bool Player::send_login_info_packet(bool b)
{
	SC_LOGIN_INFO_PACKET p;
	p.size = sizeof(SC_LOGIN_INFO_PACKET);
	p.success = true;
	p.type = SC_LOGININFO;
	if (true == b) { // �����ߴٸ�
		PlayerInfo pi;
		pi.id = id;
		pi.x = Utility::GetRandom(0.f, 100.f);
		pi.y = Utility::GetRandom(0.f, 100.f);
		pi.z = Utility::GetRandom(100.f, 200.f);
		pi.yaw = 0.f;
		
		p.player = pi;
	}
	send(&p);

	return true;
}

void Player::send(void* packet)
{
	EXT_OVER* ov = new EXT_OVER();

	// ��Ŷ ũ�� ����
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

void Player::handle_packet(char* packet, unsigned short length) // ��Ŷ ó���ϴ� �Լ�
{
    char type = packet[2];

    switch (type)
    {
	case CS_LOGIN:// ok
	{
		CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
		cout << "RECV-CS_LOGIN_PACKET: " << id << "���� " << length << "��ŭ ����!" << endl;
		// todo: db �����ؾ� ��
		name = p->name;

		// �������� �÷��̾����� Ȯ��
		// todo: ���� ��� ó������ ����ϱ� -> ������ ����� �������� ����
		bool success = true;
		for (int i = 0; i < players.size(); ++i)
			if (players[i].get_state() != NONE) {
				success = false;
				break;
			}
		send_login_info_packet(success);

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
	case CS_SPAWN:
	{
		break;
	}
    default:

        break;
    }
}
