#pragma once

#include "Quest.h"
#include "Room.h"
#include "Party.h"
#include "Kid.h"

// todo: 락 혹은 concurrency로 모두모두 변경해야 함
enum STATES { NONE = 0, CONNECTING = 1, PLAYING = 2 };
enum LOCATION { WORLD = 0, HOME = 1 };
class Player
{
	// 네트워크 통신 관련
	EXT_OVER over;
	SOCKET socket;
	std::mutex socket_lock;
	std::vector<char> packet_data; // deque를 사용할까?
	
	// 플레이어 정보
	std::string id; // 접속용 id
	STATES state; // 세션상태
	LOCATION location = WORLD; // 현재 위치
	PlayerInfo pinfo; // 플레이어 정보
	Customizing custom; // 커스터마이징 정보
	std::wstring name = L""; // 플레이어 이름

	std::unordered_map<unsigned short, unsigned short> player_item;
	std::vector<Quest> quests;
	Room* room;

	// 동기화 관련
	std::mutex info_lock;

	// 파티
	Party* party = nullptr; // 파티 정보

	// 패킷 send 함수
	bool send_login_info_packet(bool res, bool isnew);
	bool send_enter_game_packet();
	bool send_spawn_packet(PlayerInfo pi, Customizing cus, std::wstring name);
	bool send_despawn_packet(int id);
	bool send_move_packet(PlayerInfo pi);
	bool send_chat_packet(std::wstring name, std::wstring chat, unsigned int id);
	bool send_update_item_packet(unsigned short id, unsigned short num); // 해당 아이템이 num개로 업데이트
	bool send_update_gold(int sc_gold_offset);
	bool send_get_quest_packet(unsigned short gid, unsigned short n);
	bool send_remove_quest_packet(unsigned short gid, unsigned short n);
	bool send_room_leave_packet();
	bool send_update_party_packet();
	bool send_invite_call_packet(std::string& id, std::wstring& name);
	bool send_join_success_packet(std::string& id, std::wstring& name);
	bool send_doors_state_packet();
	bool send_spawn_npcs_packet();
	bool send_spawn_npc_packet(Kid k);
	void handle_disconnect();

	// 핸들러 함수
	void handle_party_packet(CS_UPDATE_PARTY_PACKET& pkt);

public:
	bool send_time_sync_packet();


	Player() : socket(0), state(NONE) {}
	Player(SOCKET s, int id);
	~Player() {}

	void init_player(SOCKET s, int id);

	void send(void* packet);
	void recv();
	void handle_packet(char* packet, unsigned short length);
	void player_setup();

	void update_packet(EXT_OVER*& ov, DWORD num_bytes)
	{
		size_t current_size = packet_data.size();
		packet_data.resize(current_size + num_bytes);
		memcpy(packet_data.data() + current_size, ov->wb_buf, num_bytes);
	}

	void process_buffer()
	{
		if (packet_data.size() < 2) return;
		unsigned short expected_packet_size;
		memcpy(&expected_packet_size, packet_data.data(), 2);

		while (packet_data.size() >= expected_packet_size) {
			handle_packet(packet_data.data(), expected_packet_size);
			packet_data.erase(packet_data.begin(), packet_data.begin() + expected_packet_size);

			if (packet_data.size() < 2) return;
			expected_packet_size = packet_data[0] + packet_data[1];
		}
	}
	bool save_db_pinfo();
	bool save_db_pInventory();

	// 추가적인 기능을 위해 getter와 setter를 추가할 수 있습니다.
	SOCKET get_socket() const { return socket; }
	void set_socket(SOCKET sock) { socket = sock; }

	STATES get_state() const { return state; }
	void set_state(STATES st) { state = st; }

	int Get_id();
};

