#include "stdafx.h"
#include "DBManager.h"
#include "RoomManager.h"
#include "Player.h"

Player::Player(SOCKET s, int id)
	: socket(s)
{
	pinfo.id = id;
}

void Player::init_player(SOCKET s, int id)
{
	socket = s;
	pinfo.id = id;
}

int Player::Get_id()
{
	return pinfo.id;
}

bool Player::send_login_info_packet(bool res, bool isnew)
{
	SC_LOGIN_INFO_PACKET p;
	p.size = sizeof(SC_LOGIN_INFO_PACKET);
	p.success = res;
	p.type = SC_LOGININFO;
	p.is_new = isnew;

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
	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);

	p.time = static_cast<float>(std::fmod(f_time + (elapsed.count() * 0.0001f), 24));

	for (int i = 0; i < ITEM_SIZE; ++i) {
		if (true == player_item.contains(i)) p.items[i] = player_item[i];
		else p.items[i] = 0;
	}
	
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

bool Player::send_spawn_packet(PlayerInfo pi, Customizing cus, std::wstring name)
{
	SC_SPAWN_PACKET p;
	p.size = sizeof(SC_SPAWN_PACKET);
	p.type = SC_SPAWN;
	p.pl = pi;
	p.c = cus;
	wcsncpy_s(p.name, sizeof(p.name) / sizeof(wchar_t), name.c_str(), _TRUNCATE); // name
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

bool Player::send_chat_packet(std::wstring name, std::wstring chat, unsigned int id)
{
	SC_CHAT_PACKET p;
	p.size = sizeof(SC_CHAT_PACKET);
	p.type = SC_CHAT;
	p.from_id = id;

	wcsncpy_s(p.name, sizeof(p.name) / sizeof(wchar_t), name.c_str(), _TRUNCATE); // name 
	wcsncpy_s(p.msg, sizeof(p.msg) / sizeof(wchar_t), chat.c_str(), _TRUNCATE); // chat 

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

	send(&p);
	return true;
}

bool Player::send_update_gold(int sc_gold)
{
	SC_UPDATE_GOLD_PACKET p;
	p.size = sizeof(SC_UPDATE_GOLD_PACKET);
	p.type = SC_UPDATE_GOLD;
	p.gold = sc_gold;
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
	send(&p);
	return true;
}

bool Player::send_room_leave_packet()
{
	SC_ROOM_LEAVE_PACKET p;
	p.size = sizeof(SC_ROOM_LEAVE_PACKET);
	p.type = SC_ROOM_LEAVE;
	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);

	p.time = static_cast<float>(std::fmod(f_time + (elapsed.count() * 0.0001f), 24));
	p.x = 960;
	p.y = 1650;
	p.z = 3300;
	send(&p);
	return true;
}

bool Player::send_time_sync_packet()
{
	SC_TIME_SYNC_PACKET p;
	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
	p.curtime = static_cast<float>(std::fmod(f_time + (elapsed.count() * 0.0001f), 24));
	p.size = sizeof(SC_TIME_SYNC_PACKET);
	p.type = SC_TIME_SYNC;
	
	send(&p);
	return true;
}

bool Player::send_update_party_packet()
{
	SC_UPDATE_PARTY_PACKET p;
	p.size = sizeof(SC_UPDATE_PARTY_PACKET);
	p.type = SC_UPDATE_PARTY; 
	p.act_type = PARTY_REQUEST::PARTY_UPDATE;
	p.member_count = party->get_member_count();
	for (int i = 0; i < MAX_PARTY_MEMBER; ++i) {
		if (i < party->get_member_count()) {
			if (party->get_members()[i] == this) continue;
			wcscpy_s(p.membersName[i], sizeof(p.membersName[i]) / sizeof(wchar_t), party->get_members()[i]->name.c_str());
		}
		else {
			wcscpy_s(p.membersName[i], sizeof(p.membersName[i]) / sizeof(wchar_t), L"");
		}
	}

	send(&p);
	return true;
}

bool Player::send_invite_call_packet(std::string& id, std::wstring& name)
{
	SC_RESULT_PARTY_PACKET p;
	p.type = SC_RESULT_PARTY;
	strcpy_s(p.id, M_ID_SIZE, id.c_str());
	p.act_type = PARTY_REQUEST::PARTY_REQUEST_INVITE;
	p.size = sizeof(SC_RESULT_PARTY_PACKET);
	wcsncpy_s(p.name, sizeof(p.name) / sizeof(wchar_t), name.c_str(), _TRUNCATE);

	std::cout << "[DEBUG] send_invite_call_packet called for id: " << id << std::endl;
	send(&p);
	return true;
}

bool Player::send_join_success_packet(std::string& id, std::wstring& name)
{
	SC_RESULT_PARTY_PACKET p;
	p.type = SC_RESULT_PARTY;
	strcpy_s(p.id, M_ID_SIZE, id.c_str());
	p.act_type = PARTY_REQUEST::PARTY_JOIN_SUCCESS;
	p.size = sizeof(SC_RESULT_PARTY_PACKET);
	strcpy_s(p.channel_id, M_ID_SIZE, party->get_partyID().c_str());
	wcsncpy_s(p.name, sizeof(p.name) / sizeof(wchar_t), name.c_str(), _TRUNCATE);

	send(&p);
	return true;
}

bool Player::send_doors_state_packet()
{
	SC_UPDATE_DOORS_PACKET p;
	p.size = sizeof(SC_UPDATE_DOORS_PACKET);
	p.type = SC_DOORS_UPDATE;
	for (int i = 0; i < MAX_DOOR; ++i) {
		p.door_id[i] = i;
		p.is_open[i] = doors[i].is_open;
	}

	send(&p);
	return true;
}

bool Player::send_spawn_npcs_packet()
{
	int count = min(npc_count, MAX_NPC);
	int packetSize = sizeof(SC_SPAWN_NPCS_PACKET) + sizeof(NPCUnitData) * count;

	char* buffer = new char[packetSize];

	SC_SPAWN_NPCS_PACKET* p = reinterpret_cast<SC_SPAWN_NPCS_PACKET*>(buffer);
	p->type = SC_NPCS_SPAWN;
	p->npc_count = count;
	p->size = packetSize;

	NPCUnitData* npc_array = reinterpret_cast<NPCUnitData*>(buffer + sizeof(SC_SPAWN_NPCS_PACKET));
	for (int i = 0; i < count; ++i)
	{
		auto& npc = npcs[i];
		npc_array[i].id = npc.id;
		npc_array[i].preg_id = npc.preg_id;
		npc_array[i].spouse_id = npc.spouse_id;
		npc_array[i].c = npc.customizing;
		npc_array[i].x = npc.x;
		npc_array[i].y = npc.y;
		npc_array[i].z = npc.z;
		npc_array[i].yaw = npc.yaw;
		npc_array[i].personality = npc.personality;
		npc_array[i].is_kid = npc.is_kid;
		wcsncpy_s(npc_array[i].name, sizeof(npc_array[i].name) / sizeof(wchar_t), npc.name.c_str(), _TRUNCATE);
		wcsncpy_s(npc_array[i].hello_msg, sizeof(npc_array[i].hello_msg) / sizeof(wchar_t), npc.hello_msg.c_str(), _TRUNCATE);	
	}

	send(buffer);
	return true;
}


bool Player::send_spawn_npc_packet(int id) 
{
	auto target = npcs[id];

	SC_SPAWN_NPC_PACKET p;
	p.size = sizeof(SC_SPAWN_NPC_PACKET);
	p.type = SC_NPC_SPAWN;
	p.id = id;
	p.preg_id = target.preg_id;
	p.spouse_id = target.spouse_id;
	p.c = target.customizing;
	p.x = target.x;
	p.y = target.y;
	p.z = target.z;
	p.yaw = target.yaw;
	p.personality = target.personality;
	p.is_kid = target.is_kid;

	send(&p);
	return true;
}


bool Player::send_reject_call_packet(std::string& id)
{
	SC_RESULT_PARTY_PACKET p;
	p.type = SC_RESULT_PARTY;
	strcpy_s(p.id, M_ID_SIZE, id.c_str());
	p.act_type = PARTY_REQUEST::PARTY_REQUEST_INVITE_REJECT;
	p.size = sizeof(SC_RESULT_PARTY_PACKET);

	send(&p);
	return true;
}


void Player::handle_party_packet(CS_UPDATE_PARTY_PACKET& pkt)
{
	switch (pkt.act_type)
	{
	case PARTY_REQUEST::PARTY_REQUEST_INVITE:
	{
		if (party == nullptr) {
			party = new Party();
			party->set_partyID(id);
			party->add_member(this);
			send_join_success_packet(id, name);
		}
		if (party->get_member_count() >= MAX_PARTY_MEMBER) return; 

		for (int i = 0; i < g_player_count; ++i) {
			if (players[i].get_state() == PLAYING && strcmp(players[i].id.c_str(), pkt.id) == 0) {
				players[i].send_invite_call_packet(id, name);
				break;
			}
		}
		break;
	}
	case PARTY_REQUEST::PARTY_REQUEST_INVITE_ACCEPT:
	{
		for (int i = 0; i < g_player_count; ++i) {
			if (players[i].get_state() == PLAYING && strcmp(players[i].id.c_str(), pkt.id) == 0) {
				players[i].party->add_member(this);
				party = players[i].party;
				//TODO 초대를 보낸놈한테 수락됏다고 알려줘야하지않나?

				break;
			}
		}

		for (auto& a : party->get_members()) {
			a->send_update_party_packet();
		}
		break;
	}
	case PARTY_REQUEST::PARTY_REQUEST_INVITE_REJECT:
	{
		break;
	}
	}
}

bool Player::save_db_pinfo()
{
	DBManager::SavePInfo(this->id, this->pinfo);
	return true;
}

bool Player::save_db_pInventory()
{
	for (auto& a : player_item) {
		DBManager::SaveItem(this->id, a.first, a.second);
	}
	return true;
}

void Player::send(void* packet)
{
	std::lock_guard ll{ socket_lock };

	if (socket == INVALID_SOCKET)
	{
		printf("[send] Invalid socket. Send aborted.\n");
		return;
	}

	unsigned short p_size;
	memcpy(&p_size, packet, 2);

	EXT_OVER* ov = new EXT_OVER();
	ov->setup_send(reinterpret_cast<char*>(packet), p_size);

	int result = WSASend(socket, &ov->wsabuf, 1, 0, 0, &ov->over, NULL);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
		{
			printf("WSASend failed with error: %d\n", error);

			closesocket(socket);
			socket = INVALID_SOCKET;

			delete ov;
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
	over.wsabuf.len = static_cast<ULONG>(BUFSIZE - packet_data.size());
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
			std::lock_guard ll{ socket_lock };
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
		int id = pinfo.id;
		CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
		std::cout << "[DEBUG] Player " << p->id << " is trying to log in." << std::endl;

		bool is_new = false;
		bool success = true;
		
		if (DBManager::checkLogin(p->id, p->pw, is_new)) {
			if (false == is_new) {
				std::lock_guard ll{ info_lock };
				DBManager::LoadPInfo(p->id, pinfo, this->name);
				DBManager::LoadCustomizing(p->id, this->custom);
				DBManager::LoadItem(p->id, player_item);
				DBManager::LoadQuest(p->id, quests);
				pinfo.gold = DBManager::LoadGold(p->id);
				std::string tmpstr = p->id;
				room = RoomManager::Get()[tmpstr];
				this->id = p->id;
			}
			else {
				std::lock_guard ll{ info_lock };
				this->id = p->id;
				player_setup();
				RoomManager::Get().insert({ p->id, new Room() });
				room = RoomManager::Get()[p->id];
				room->SaveToDB(p->id);
				room->RemoveObjectByPosition(0.0f, 0.0f, 0.0f);
			}
		}
		else {
			success = false;
		}

		for (int i = 0; i < g_player_count; ++i) {
			if (players[i].get_state() != NONE && players[i].id == p->id) {
				success = false;
				break;
			}
		}
		
		send_login_info_packet(success, is_new);
		break;
    }
	case CS_ENTER_GAME:
	{
		CS_ENTER_GAME_PACKET* p = reinterpret_cast<CS_ENTER_GAME_PACKET*>(packet);
		std::cout << "[DEBUG] Player " << id << " is entering the game." << std::endl;

		if (this->name == L"") {
			{
				std::lock_guard ll{ info_lock };
				this->name = p->name;
			}
			DBManager::SavePname(this->id, this->name);
		}
		state = PLAYING;
		
		for (auto& a : player_item) {
			send_update_item_packet(a.first, a.second);
		}

		send_enter_game_packet();
		send_doors_state_packet();
		send_spawn_npcs_packet();

		{
			std::lock_guard<std::mutex> lock(players_mutex);
			for (int i = 0; i < g_player_count; ++i) {
				if (players[i].get_state() == PLAYING and players[i].id != this->id and players[i].pinfo.st < HOME) {
					players[i].send_spawn_packet(pinfo, custom, name);
				}
			}

			for (int i = 0; i < g_player_count; ++i) {
				if (players[i].get_state() == PLAYING and players[i].id != this->id and players[i].pinfo.st < HOME) {
					send_spawn_packet(players[i].pinfo, players[i].custom, players[i].name);
				}
			}
		}
		break;
	}
    case CS_CHAT:
    {
		int id = pinfo.id;
        CS_CHAT_PACKET* p = reinterpret_cast<CS_CHAT_PACKET*>(packet);

		std::wcout << p->name << ": " << p->msg << std::endl;

		{
			std::lock_guard<std::mutex> lock(players_mutex);
			for (int i = 0; i < g_player_count; ++i) {
				if (players[i].state == PLAYING and i != id and players[i].pinfo.st != HOME)
					players[i].send_chat_packet(p->name, p->msg, p->from_id);
			}
		}
        break;
    }
    case CS_LEAVE:
    {
		CS_LEAVE_PACKET* p = reinterpret_cast<CS_LEAVE_PACKET*>(packet);

		std::cout << "[DEBUG] Player " << id << " is leaving the game." << std::endl;

		if (state != PLAYING) {
			state = NONE;
			break;
		}

		//{
		//	if (party != nullptr) {
		//		party->remove_member(this);
		//		if (party->get_member_count() == 0) {
		//			delete party;
		//			party = nullptr;
		//		}
		//		else {
		//			for (auto& a : party->get_members()) {
		//				a->send_update_party_packet();
		//			}
		//		}
		//	}
		//}

		{
			std::lock_guard<std::mutex> lock(players_mutex);
			for (int i = 0; i < g_player_count; ++i) {
				if (players[i].state == PLAYING and i != pinfo.id)
					players[i].send_despawn_packet(pinfo.id);
			}
		}
		
		DBManager::SavePInfo(this->id, this->pinfo);

		state = NONE;
        break;
    }
	case CS_MOVEP:
	{
		CS_MOVE_PACKET* p = reinterpret_cast<CS_MOVE_PACKET*>(packet);
		
		{
			std::lock_guard<std::mutex> lock(info_lock);
			pinfo.feel = p->pl.feel;
			pinfo.x = p->pl.x;
			pinfo.y = p->pl.y;
			pinfo.z = p->pl.z;
			pinfo.yaw = p->pl.yaw;
			pinfo.st = p->pl.st;
		}

		{
			std::lock_guard<std::mutex> lock(players_mutex);

			if (this->location == HOME) 
			{
				for (int i = 0; i < room->players.size(); ++i) {
					if (this != room->players[i]) {
						room->players[i]->send_move_packet(p->pl);
					}
				}
			}
			else 
			{
				for (int i = 0; i < g_player_count; ++i) {
					if (players[i].state == PLAYING and players[i].location == WORLD)
						players[i].send_move_packet(p->pl);
				}
			}
		}
		break;
	}
	case CS_GET_ITEM:
	{
		int id = pinfo.id;
		CS_GET_ITEM_PACKET* p = reinterpret_cast<CS_GET_ITEM_PACKET*>(packet);
		if (player_item.contains(p->id)) player_item[p->id] += p->num;
		else player_item[p->id] = p->num;

		save_db_pInventory();

		send_update_item_packet(p->id, player_item[p->id]);
		break;
	}
	case CS_UPDATE_CUSTOM:
	{
		CS_UPDATE_CUSTOM_PACKET* p = reinterpret_cast<CS_UPDATE_CUSTOM_PACKET*>(packet);
		this->custom = p->c;

		DBManager::SaveCustomizing(this->id, this->custom);
		break;
	}
	case CS_UPDATE_GOLD:
	{
		CS_UPDATE_GOLD_PACKET* p = reinterpret_cast<CS_UPDATE_GOLD_PACKET*>(packet);
		std::cout << "[DEBUG] Player " << this->id << "Prev: " << this->pinfo.gold << " Curr: " << this->pinfo.gold + p->gold_offset<< std::endl;
		this->pinfo.gold += p->gold_offset;
		DBManager::SaveGold(this->id, this->pinfo.gold);
		send_update_gold(this->pinfo.gold);
		break;
	}
	case CS_GET_QUEST:
	{
		CS_UPDATE_QUEST_PACKET* p = reinterpret_cast<CS_UPDATE_QUEST_PACKET*>(packet);
		this->quests.emplace_back(p->giver_id, p->num);
		Quest q{ p->giver_id, p->num };
		
		DBManager::SaveQuest(this->id, q);

		send_get_quest_packet(p->giver_id, p->num);

		break;
	}
	case CS_REMOVE_QUEST:
	{
		CS_UPDATE_QUEST_PACKET* p = reinterpret_cast<CS_UPDATE_QUEST_PACKET*>(packet);

		for (int i = 0; i < this->quests.size(); ++i) {
			if (this->quests[i].GetNUM() == p->num) {
				auto q = this->quests[i].GetQuestInfo();
				DBManager::DeleteQuest(this->id, q);
				this->quests.erase(this->quests.begin() + i);
				break;
			}
		}

		send_remove_quest_packet(p->giver_id, p->num);
		break;
	}
	case CS_ROOM_ENTER: // todo: 가구 편집할 때 다른 플레이어들한테 브로드캐스팅
	{
		CS_ROOM_ENTER_PACKET* p = reinterpret_cast<CS_ROOM_ENTER_PACKET*>(packet);

		SC_ROOM_SETUP_PACKET pkt;
		pkt.type = SC_ROOM_SETUP;
		strcpy_s(pkt.id, M_ID_SIZE, p->id);
		RoomManager::Get()[p->id]->packet_setup(pkt);
		pkt.size = sizeof(SC_ROOM_SETUP_PACKET);
		
		if (location != HOME) {
			location = HOME;

			room = RoomManager::Get()[p->id];

			{
				std::lock_guard<std::mutex> lock(players_mutex);
				for (int i = 0; i < g_player_count; ++i) {
					if (players[i].state == PLAYING and i != pinfo.id and players[i].location == WORLD)
						players[i].send_despawn_packet(pinfo.id);
				}
			}

			room->AddPlayer(this);

			{
				for (int i = 0; i < room->players.size(); ++i) {
					if (room->players[i] != this)
						send_spawn_packet(room->players[i]->pinfo, room->players[i]->custom, room->players[i]->name);
				}
			}

			{
				std::lock_guard ll{ room->m };
				for (int i = 0; i < room->players.size(); ++i) {
					if (room->players[i] != this)
						room->players[i]->send_spawn_packet(this->pinfo, this->custom, this->name);
				}
			}
		}

		send(&pkt);
		break;
	}
	case CS_PLACE_BUILD: // todo: 여기 테스트
	{
		CS_PLACE_BUILD_PACKET* p = reinterpret_cast<CS_PLACE_BUILD_PACKET*>(packet);
		Object obj;
		obj.item_id = p->build.item_id;
		obj.x = p->build.x;
		obj.y = p->build.y;
		obj.z = p->build.z;
		obj.yaw = p->build.yaw;
		obj.scale = p->build.scale;

		room->AddObject(obj);

		SC_ROOM_SETUP_PACKET pkt;
		pkt.type = SC_ROOM_SETUP;
		strcpy_s(pkt.id, M_ID_SIZE, room->ownerID.c_str());
		room->packet_setup(pkt);
		pkt.size = sizeof(SC_ROOM_SETUP_PACKET);

		{
			std::lock_guard ll{ room->m };
			for (int i = 0; i < room->players.size(); ++i) {
				if (room->players[i] != this)
					room->players[i]->send(&pkt);
			}
		}
		break;
	}
	case CS_REMOVE_BUILD:
	{
		CS_REMOVE_BUILD_PACKET* p = reinterpret_cast<CS_REMOVE_BUILD_PACKET*>(packet);

		room->RemoveObjectByPosition(p->x, p->y, p->z);

		SC_ROOM_SETUP_PACKET pkt;
		pkt.type = SC_ROOM_SETUP;
		strcpy_s(pkt.id, M_ID_SIZE, room->ownerID.c_str());
		room->packet_setup(pkt);
		pkt.size = sizeof(SC_ROOM_SETUP_PACKET);

		{
			std::lock_guard ll{ room->m };
			for (int i = 0; i < room->players.size(); ++i) {
				if (room->players[i] != this)
					room->players[i]->send(&pkt);
			}
		}
		break;
	}
	case CS_UPDATE_BUILD:
	{
		CS_UPDATE_BUILD_PACKET* p = reinterpret_cast<CS_UPDATE_BUILD_PACKET*>(packet);

		room->UpdateObjectTransform(p->old_x, p->old_y, p->old_z, p->new_x, p->new_y, p->new_z, p->new_yaw);

		SC_ROOM_SETUP_PACKET pkt;
		pkt.type = SC_ROOM_SETUP;
		strcpy_s(pkt.id, M_ID_SIZE, room->ownerID.c_str());
		room->packet_setup(pkt);
		pkt.size = sizeof(SC_ROOM_SETUP_PACKET);

		{
			std::lock_guard ll{ room->m };
			for (int i = 0; i < room->players.size(); ++i) {
				if (room->players[i] != this)
					room->players[i]->send(&pkt);
			}
		}
		break;
	}
	case CS_ROOM_LEAVE:
	{
		CS_ROOM_LEAVE_PACKET* p = reinterpret_cast<CS_ROOM_LEAVE_PACKET*>(packet);

		if (this->id == room->ownerID)
		{
			DBManager::DeleteRoomObjects(id);
			room->SaveToDB(id);
		}

		this->location = WORLD;

		{
			std::lock_guard ll{ room->m };

			for (Player* p : room->players)
			{
				if (p != this)
					p->send_despawn_packet(pinfo.id);
			}
		}
		room->RemovePlyer(pinfo.id); 
		this->room = nullptr; 

		send_room_leave_packet();

		{
			std::lock_guard<std::mutex> lock(players_mutex);
			for (int i = 0; i < g_player_count; ++i) {
				if (players[i].get_state() == PLAYING and players[i].id != this->id and players[i].location == WORLD) {
					players[i].send_spawn_packet(pinfo, custom, this->name);
				}
			}

			for (int i = 0; i < g_player_count; ++i) {
				if (players[i].get_state() == PLAYING and players[i].id != this->id and players[i].location == WORLD) {
					send_spawn_packet(players[i].pinfo, players[i].custom, players[i].name);
				}
			}
		}

		break;
	}
	case CS_UPDATE_PARTY:
	{
		CS_UPDATE_PARTY_PACKET* p = reinterpret_cast<CS_UPDATE_PARTY_PACKET*>(packet);

		handle_party_packet(*p);
		break;
	}
	case CS_ADD_KID: 
	{
		CS_ADD_KID_PACKET* p = reinterpret_cast<CS_ADD_KID_PACKET*>(packet);
		Kid temp_kid(*p);
		temp_kid.id = npc_count++;
		temp_kid.is_kid = true; 
		DBManager::SaveKidInfo(temp_kid);

		// todo: 다시 엔피씨들 넣기.
		break;
	}
	case CS_DOOR_UPDATE:
	{
		CS_UPDATE_DOOR_PACKET* p = reinterpret_cast<CS_UPDATE_DOOR_PACKET*>(packet);
		doors[p->door_id].is_open = p->is_open;

		SC_UPDATE_DOOR_PACKET pkt;
		pkt.size = sizeof(SC_UPDATE_DOOR_PACKET);
		pkt.type = SC_DOOR_UPDATE;
		pkt.door_id = p->door_id;
		pkt.is_open = p->is_open;

		for (int i = 0; i < g_player_count; ++i) {
			if (players[i].get_state() == PLAYING and players[i].id != this->id) {
				players[i].send(&pkt);
			}
		}
		break;
	}
	case CS_TEST_MOVE:
	{
		CS_TEST_MOVE_PACKET* p = reinterpret_cast<CS_TEST_MOVE_PACKET*>(packet);
		pinfo = p->pl;

		SC_TEST_MOVE_PACKET resp;
		resp.size = sizeof(SC_TEST_MOVE_PACKET);
		resp.type = SC_TEST_MOVE;
		resp.client_send_time = p->client_send_time;
		resp.server_send_time = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::steady_clock::now().time_since_epoch()).count();  // 서버 현재 시간 추가
		resp.pl = pinfo;

		{
			std::lock_guard<std::mutex> lock(players_mutex);

			for (int i = 0; i < g_player_count; ++i) {
				if (players[i].state == PLAYING and players[i].location == WORLD)
					players[i].send(&resp);
			}
		}

		send(&resp); // 클라로 응답
		break;
	}
    default:
        break;
    }
}

void Player::player_setup() 
{
	pinfo.x = 960 + Utility::GetRandom(100.0f, 200.0f);
	pinfo.y = 1650 + Utility::GetRandom(100.0f, 200.0f);
	pinfo.z = 3200;
	pinfo.yaw = 0.f;

	player_item[8] = 1;
	player_item[9] = 1;
	player_item[10] = 1;
	player_item[11] = 1;

	pinfo.gold = 1500; 

	DBManager::SaveDefPInfo(this->id, pinfo);
	DBManager::SaveDefCustomizing(this->id);
	save_db_pInventory();
}
