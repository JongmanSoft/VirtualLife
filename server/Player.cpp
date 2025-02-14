#include "stdafx.h"
#include "Player.h"

bool Player::send_login_info_packet(bool b, PlayerInfo pi)
{
	SC_LOGIN_INFO_PACKET p;
	p.size = sizeof(SC_LOGIN_INFO_PACKET);
	p.success = true;
	p.type = SC_LOGININFO;
	if (true == b) { // 성공했다면
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

bool Player::send_despawn_packet(int id)
{
	SC_DESPAWN_PACKET p;
	p.size = sizeof(SC_DESPAWN_PACKET);
	p.type = SC_DESPAWN;
	p.id = id;
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

bool Player::send_chat_packet(string name, wstring chat)
{
	SC_CHAT_PACKET p;
	p.size = sizeof(SC_CHAT_PACKET);
	p.type = SC_CHAT;

	// 안전하게 문자열 복사
	strncpy_s(p.name, sizeof(p.name), name.c_str(), _TRUNCATE);      // name 복사
	wcsncpy_s(p.msg, sizeof(p.msg) / sizeof(wchar_t), chat.c_str(), _TRUNCATE);  // chat 복사

	send(&p);
	return true;
}

bool Player::send_update_item_packet(unsigned short id, unsigned short num) 
{
	SC_UPDATE_ITEM_PACKET p;
	p.size = sizeof(SC_UPDATE_ITEM_PACKET);
	p.type = SC_UPDATE_ITEM;
	p.id = id;
	p.num = num;

	cout << this->id << "에게 SC_UPDATE_ITEM_PACKET 보냄: " << id << "번 아이템이 " << num << "개로 변화!" << endl;
	send(&p);
	return true;
}

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
	cout << "SEND: " << id << "에게 패킷 전송!" << endl;
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

void Player::handle_packet(char* packet, unsigned short length) // 패킷 처리하는 함수
{
    char type = packet[2];

    switch (type)
    {
	case CS_LOGIN:// ok
	{
		CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
		cout << "RECV-CS_LOGIN_PACKET: " << id << "에게 " << length << "만큼 받음!" << endl;
		// todo: db 연동해야 함

		// 접속중인 플레이어인지 확인
		// todo: 여기 어떻게 처리할지 고민하기 -> 개선의 방법이 여러가지 있음
		bool success = true;
		for (int i = 0; i < players.size(); ++i)
			if (players[i].get_state() != NONE && players[i].name == p->name) {
				success = false;
				break;
			}
		
		name = p->name;
		PlayerInfo pi;
		if (true == success) {
			pi.id = id;
			pi.x = Utility::GetRandom(0.f, 200.0f);
			pi.y = Utility::GetRandom(0.f, 200.0f);
			pi.z = 200.0f;
			pi.yaw = 0.f;
			pinfo = pi;

			send_login_info_packet(success, pi);

			// 지금 있는 애들한테 브로드캐스팅
			// 지금 있는 애들 정보 받아오기
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

		// 보낸 채팅 확인용
		std::wcout << p->name << ": " << p->msg << std::endl;

		// 채팅 브로드캐스트
		for (int i = 0; i < players.size(); ++i) {
			if (players[i].state == PLAYING and i != id)
				players[i].send_chat_packet(p->name, p->msg);
		}
        break;
    }
    case CS_LEAVE:
    {
		CS_LEAVE_PACKET* p = reinterpret_cast<CS_LEAVE_PACKET*>(packet);

		if (state != PLAYING) {
			state = NONE;
			break; // 로그인 밴 막기
		}

		// 나간 플레이어 정보 브로드캐스팅
		for (int i = 0; i < players.size(); ++i) {
			if (players[i].state == PLAYING and i != id)
				players[i].send_despawn_packet(id);
		}
		
		cout << id << "가 종료!" << endl;

		state = NONE;
        break;
    }
	case CS_MOVEP:
	{
		CS_MOVE_PACKET* p = reinterpret_cast<CS_MOVE_PACKET*>(packet);
		pinfo = p->pl;

		// 위치정보 브로드캐스팅
		for (int i = 0; i < players.size(); ++i) {
			if (players[i].get_state() == PLAYING and i != id) {
				players[i].send_move_packet(p->pl);
			}
		}

		break;
	}
	case CS_GET_ITEM:
	{
		CS_GET_ITEM_PACKET* p = reinterpret_cast<CS_GET_ITEM_PACKET*>(packet);
		if (player_item.contains(p->id)) player_item[id] += p->num;
		else player_item[id] = p->num;

		send_update_item_packet(id, player_item[id]);
	}
    default:

        break;
    }
}
