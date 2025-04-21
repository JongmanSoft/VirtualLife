#include "stdafx.h"
#include "DBManager.h"
#include "Player.h"

// todo: 시간!

bool Player::send_login_info_packet(bool res, bool isnew)
{
	SC_LOGIN_INFO_PACKET p;
	p.size = sizeof(SC_LOGIN_INFO_PACKET);
	p.success = res;
	p.type = SC_LOGININFO;
	//p.isNew = isnew;

	send(&p);

	return true;
}

bool Player::send_enter_game_packet()
{
	SC_ENTER_GAME_PACKET p;
	p.size = sizeof(SC_ENTER_GAME_PACKET);
	p.custom = custom;
	p.player = pinfo;
	wcsncpy_s(p.name, sizeof(p.name) / sizeof(wchar_t), name.c_str(), _TRUNCATE);
	p.type = SC_ENTER_GAME;
	
	// 인벤토리 초기화
	for (int i = 0; i < ITEM_SIZE; ++i) {
		if (true == player_item.contains(i)) p.items[i] = player_item[i];
		else p.items[i] = 0;
	}
	
	// 퀘스트 초기화
	for (int i = 0; i < QUEST_MAX; ++i) {
		if (quests.size() <= i) {
			p.giver_id[i] = -1;
			p.num[i] = -1;
		}
		else {
			p.giver_id[i] = quests[i].GetGID();
			p.num[i] = quests[i].GetNUM();
		}
	}

	send(&p);

	return true;
}

bool Player::send_spawn_packet(PlayerInfo pi, Customizing cus)
{
	SC_SPAWN_PACKET p;
	p.size = sizeof(SC_SPAWN_PACKET);
	p.type = SC_SPAWN;
	p.pl = pi;
	p.c = cus;
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

bool Player::send_chat_packet(std::wstring name, std::wstring chat)
{
	SC_CHAT_PACKET p;
	p.size = sizeof(SC_CHAT_PACKET);
	p.type = SC_CHAT;

	// 안전하게 문자열 복사
	wcsncpy_s(p.name, sizeof(p.name) / sizeof(wchar_t), name.c_str(), _TRUNCATE); // name 복사
	wcsncpy_s(p.msg, sizeof(p.msg) / sizeof(wchar_t), chat.c_str(), _TRUNCATE); // chat 복사

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

	std::cout << pinfo.id << "에게 SC_UPDATE_ITEM_PACKET 보냄: " << id << "번 아이템이 " << num << "개로 변화!" << std::endl;
	send(&p);
	return true;
}

bool Player::send_update_gold(int sc_gold)
{
	SC_UPDATE_GOLD_PACKET p;
	p.size = sizeof(SC_UPDATE_GOLD_PACKET);
	p.type = SC_UPDATE_GOLD;
	p.gold = sc_gold;
	std::cout << pinfo.id << "에게 SC_GOLD_UPDATE 보냄: " << sc_gold<<"원이 됨!" << std::endl;
	send(&p);
	return true;
}

bool Player::send_get_quest_packet(unsigned short gid, unsigned short n)
{
	SC_UPDATE_QUEST_PACKET p;
	p.size = sizeof(SC_UPDATE_QUEST_PACKET);
	p.type = SC_GET_QUEST;
	p.giver_id = gid;
	p.num = n;
	std::cout << pinfo.id << "에게 SC_UPDATE_QUEST 보냄: " << gid << "에게 " << n << "번 퀘스트를 받음" << std::endl;
	send(&p);
	return true;
}

bool Player::send_remove_quest_packet(unsigned short gid, unsigned short n)
{
	SC_UPDATE_QUEST_PACKET p;
	p.size = sizeof(SC_UPDATE_QUEST_PACKET);
	p.type = SC_REMOVE_QUEST;
	p.giver_id = gid;
	p.num = n;
	std::cout << pinfo.id << "에게 SC_UPDATE_QUEST 보냄: " << gid << "에게 받은" << n << "번 퀘스트가 없어짐" << std::endl;
	send(&p);
	return true;
}

bool Player::send_room_setup_packet()
{
	SC_ROOM_SETUP_PACKET p;
	p.size = sizeof(SC_ROOM_SETUP_PACKET);
	p.type = SC_ROOM_SETUP;
	p.id = pinfo.id;
	room.packet_setup(p);
	send(&p);
	return true;
}

bool Player::save_db_pinfo()
{
	DBManager::SavePInfo(this->id, this->pinfo);
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
		int id = pinfo.id;
		CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
		std::cout << "RECV-CS_LOGIN_PACKET: " << id << "에게 " << length << "만큼 받음!" << std::endl;
		// todo: db 연동해야 함

		bool is_new = false;
		bool success = true;
		// 디비 접근 라인 -> 안되면 주석 ㄱㄱ
		{
			// 1. 신규유저 확인
			if (DBManager::checkLogin(p->id, p->pw, is_new)) {
				if (false == is_new) {
					// 기존유저
					DBManager::LoadCustomizing(p->id, this->custom);
					DBManager::LoadItem(p->id, player_item);
					// todo : Load Quest
				}
				else {
					// 신규 유저
					this->id = p->id;
					player_setup();
				}
			}
			else {
				// 로그인 실패
				success = false;
			}
		}

		this->id = p->id;
		player_setup();

		// 2. 접속중인 플레이어인지 확인
		for (int i = 0; i < players.size(); ++i) {
			if (players[i].get_state() != NONE && players[i].id == p->id) {
				success = false;
				break;
			}
		}
		
		send_login_info_packet(success, is_new);
		break;
    }
	case CS_ENTER_GAME: // 게임 접속 요청
	{
		std::cout << "RECV-CS_ENTER_GAME_PACKET: " << pinfo.id << "에게 " << length << "만큼 받음!" << std::endl;
		CS_ENTER_GAME_PACKET* p = reinterpret_cast<CS_ENTER_GAME_PACKET*>(packet);
		this->name = p->name;
		state = PLAYING;
		send_enter_game_packet();
		
		// 기존유저들에게 스폰요청
		for (int i = 0; i < players.size(); ++i) {
			if (players[i].get_state() == PLAYING and players[i].id != this->id) {
				players[i].send_spawn_packet(pinfo, custom);
			}
		}

		// 나에게 기존유저 스폰
		for (int i = 0; i < players.size(); ++i) {
			if (players[i].get_state() == PLAYING and players[i].id != this->id) {
				send_spawn_packet(players[i].pinfo, players[i].custom);
			}
		}

		break;
	}
    case CS_CHAT:
    {
		std::cout << "RECV-CS_CHAT_PACKET: " << pinfo.id << "에게 " << length << "만큼 받음!" << std::endl;
		int id = pinfo.id;
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
    case CS_LEAVE: // todo: 여기 왜 안들어오지 이상하다..
    {
		std::cout << "RECV-CS_LEAVE_PACKET: " << pinfo.id << "에게 " << length << "만큼 받음!" << std::endl;
		int id = pinfo.id;
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
		
		std::cout << id << "가 종료!" << std::endl;
		// db 정리 라인 - 안되면 주석 ㄱㄱ
		{
			DBManager::SavePInfo(this->id, this->pinfo);

		}

		state = NONE;
        break;
    }
	case CS_MOVEP:
	{
		int id = pinfo.id;
		CS_MOVE_PACKET* p = reinterpret_cast<CS_MOVE_PACKET*>(packet);
		std::cout << "RECV-CS_MOVE_PACKET: " << pinfo.id << "에게 " << length << "만큼 받음, 현재상태: " << int(p->pl.st) << std::endl;
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
		std::cout << "RECV-CS_GET_ITEM_PACKET: " << pinfo.id << "에게 " << length << "만큼 받음!" << std::endl;
		int id = pinfo.id;
		CS_GET_ITEM_PACKET* p = reinterpret_cast<CS_GET_ITEM_PACKET*>(packet);
		if (player_item.contains(p->id)) player_item[p->id] += p->num;
		else player_item[p->id] = p->num;

		send_update_item_packet(p->id, player_item[p->id]);
		break;
	}
	case CS_UPDATE_CUSTOM:
	{
		std::cout << "RECV-CS_UPDATE_CUSTOM_PACKET: " << pinfo.id << "에게 " << length << "만큼 받음!" << std::endl;
		CS_UPDATE_CUSTOM_PACKET* p = reinterpret_cast<CS_UPDATE_CUSTOM_PACKET*>(packet);
		this->custom = p->c;

		DBManager::SaveCustomizing(this->id, this->custom);
		break;
	}
	case  CS_UPDATE_GOLD:
	{
		CS_UPDATE_GOLD_PACKET* p = reinterpret_cast<CS_UPDATE_GOLD_PACKET*>(packet);
		this->addinfo.gold += p->gold_offset;
		send_update_gold(this->addinfo.gold);
		break;
	}
	case CS_GET_QUEST:
	{
		std::cout << "RECV-CS_GET_QUEST_PACKET: " << pinfo.id << "에게 " << length << "만큼 받음!" << std::endl;
		CS_UPDATE_QUEST_PACKET* p = reinterpret_cast<CS_UPDATE_QUEST_PACKET*>(packet);
		this->quests.emplace_back(p->giver_id, p->num);
		send_get_quest_packet(p->giver_id, p->num);

		break;
	}
	case CS_REMOVE_QUEST:
	{
		std::cout << "RECV-CS_GET_QUEST_PACKET: " << pinfo.id << "에게 " << length << "만큼 받음!" << std::endl;
		CS_UPDATE_QUEST_PACKET* p = reinterpret_cast<CS_UPDATE_QUEST_PACKET*>(packet);
		for (int i = 0; i < this->quests.size(); ++i) {
			if (this->quests[i].GetNUM() == p->num) {
				this->quests.erase(this->quests.begin() + i);
			}
		}
		send_remove_quest_packet(p->giver_id, p->num);
		break;
	}
	case CS_ROOM_ENTER:
	{
		std::cout << "CS_ENTER_ROOM 받음! id: " << pinfo.id << std::endl;

		SC_ROOM_SETUP_PACKET pkt;
		pkt.type = SC_ROOM_SETUP;
		pkt.id = pinfo.id;
		room.packet_setup(pkt);
		pkt.size = sizeof(pkt) - sizeof(pkt.objs) + sizeof(Object) * pkt.count;
		send(&pkt);
		break;
	}
	case CS_PLACE_BUILD:
	{
		// std::cout << "RECV-CS_PLACE_BUILD: " << pinfo.id << " 건물 건설 요청!" << std::endl;

		CS_PLACE_BUILD_PACKET* p = reinterpret_cast<CS_PLACE_BUILD_PACKET*>(packet);
		Object obj;
		obj.item_id = p->build.item_id;
		obj.x = p->build.x;
		obj.y = p->build.y;
		obj.z = p->build.z;
		obj.yaw = p->build.yaw;
		obj.scale = p->build.scale;

		room.AddObject(obj);

		// std::cout << "저장 완료: ID " << obj.item_id << ", Pos(" << obj.x << ", " << obj.y << ", " << obj.z << ")\n";
		break;
	}
	case CS_REMOVE_BUILD:
	{
		CS_REMOVE_BUILD_PACKET* p = reinterpret_cast<CS_REMOVE_BUILD_PACKET*>(packet);
		std::cout << "건물 삭제 요청 받음! 위치: (" << p->x << ", " << p->y << ", " << p->z << ")\n";

		room.RemoveObjectByPosition(p->x, p->y, p->z);
		break;
	}
	case CS_UPDATE_BUILD:
	{
		CS_UPDATE_BUILD_PACKET* p = reinterpret_cast<CS_UPDATE_BUILD_PACKET*>(packet);
		std::cout << "건물 수정 요청 받음! 위치: (" << p->old_x << ", " << p->old_y << ", " << p->old_z << ") → ("
			<< p->new_x << ", " << p->new_y << ", " << p->new_z << "), Yaw: " << p->new_yaw << "\n";

		room.UpdateObjectTransform(p->old_x, p->old_y, p->old_z, p->new_x, p->new_y, p->new_z, p->new_yaw);
		break;
	}
    default:
        break;
    }
}

void Player::player_setup() // 신규 플레이어 위치 등 셋업
{
	pinfo.x = 960 + Utility::GetRandom(100.0f, 200.0f);
	pinfo.y = 1650 + Utility::GetRandom(100.0f, 200.0f);
	pinfo.z = 3200;
	pinfo.yaw = 0.f;

	DBManager::SaveDefPInfo(this->id, pinfo);
	DBManager::SaveDefCustomizing(this->id);
}
