#include "stdafx.h"
#include "Player.h"

bool Player::send_login_info_packet(bool b, PlayerInfo pi)
{
	SC_LOGIN_INFO_PACKET p;
	p.size = sizeof(SC_LOGIN_INFO_PACKET);
	p.success = true;
	p.type = SC_LOGININFO;
	if (true == b) { // �����ߴٸ�
		p.player = pi;
		
	}
	else
		state = NONE;
	send(&p);

	return true;
}

bool Player::send_spawn_packet(PlayerInfo pi)
{
	SC_SPAWN_PACKET p;
	p.size = sizeof(SC_SPAWN_PACKET);
	p.type = SC_SPAWN;
	p.pl = pi;
	send(&p);
	return true;
}

bool Player::send_move_packet(PlayerInfo pi)
{
	SC_MOVE_PACKET p;
	p.size = sizeof(SC_MOVE_PACKET);
	p.type = SC_MOVEP;
	p.pl = pi;
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
	cout << "SEND: " << id << "���� ��Ŷ ����!" << endl;
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
			if (players[i].get_state() != NONE && players[i].name == p->name) {
				success = false;
				break;
			}
		
		PlayerInfo pi;
		if (true == success) {
			pi.id = id;
			pi.x = Utility::GetRandom(0.f, 200.0f);
			pi.y = Utility::GetRandom(0.f, 200.0f);
			pi.z = 200.0f;
			pi.yaw = 0.f;
			pinfo = pi;

			send_login_info_packet(success, pi);

			// ���� �ִ� �ֵ����� ��ε�ĳ����
			// ���� �ִ� �ֵ� ���� �޾ƿ���
			for (int i = 0; i < players.size(); ++i) {
				if (players[i].get_state() == PLAYING) {
					players[i].send_spawn_packet(pi);
				}
			}

			for (int i = 0; i < players.size(); ++i) {
				if (players[i].get_state() == PLAYING) {
					send_spawn_packet(players[i].pinfo);
				}
			}

			state = PLAYING;
		}

		break;
    }
    case CS_CHAT:
    {
        CS_CHAT_PACKET* p = reinterpret_cast<CS_CHAT_PACKET*>(packet);
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string utf8_msg = converter.to_bytes(p->msg);

		std::cout << p->name << ": " << utf8_msg << std::endl;

        break;
    }
    case CS_LEAVE:
    {
		CS_LEAVE_PACKET* p = reinterpret_cast<CS_LEAVE_PACKET*>(packet);
        break;
    }
	case CS_MOVEP:
	{
		CS_MOVE_PACKET* p = reinterpret_cast<CS_MOVE_PACKET*>(packet);
		pinfo = p->pl;

		// ��ġ���� ��ε�ĳ����
		for (int i = 0; i < players.size(); ++i) {
			if (players[i].get_state() == PLAYING and i != id) {
				players[i].send_move_packet(p->pl);
			}
		}

		break;
	}
    default:

        break;
    }
}
