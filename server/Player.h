#pragma once

#include "Quest.h"
#include "Room.h"
#include "Party.h"
#include "Kid.h"

// todo: 락 혹은 concurrency로 모두모두 변경해야 함
// none: 플레이중이 아님, connecting: 아직 Login success를 보내기 전, playing: 접속중
enum STATES { NONE = 0, CONNECTING = 1, PLAYING = 2 };

class Player
{
	// 네트워크 통신 관련
	EXT_OVER over;
	SOCKET socket;
	std::vector<char> packet_data; // deque를 사용할까?
	
	// 플레이어 정보
	std::string id; // 접속용 id
	std::wstring name = L""; // 플레이어 이름
	PlayerInfo pinfo; // obj id, 위치/회전정보
	AdditionalInfo addinfo; // 직업 등 정보
	STATES state; // 상태

	Customizing custom; // 커스텀
	std::unordered_map<unsigned short, unsigned short> player_item;
	std::vector<Quest> quests;
	Room room;

	float ping = 0.0f;

	// 동기화 관련
	std::mutex m;

	// ㅠ파티
	Party* party = nullptr; // 파티 정보

	// 패킷 send 함수
	bool send_login_info_packet(bool res, bool isnew);
	bool send_enter_game_packet();
	bool send_spawn_packet(PlayerInfo pi, Customizing cus);
	bool send_despawn_packet(int id);
	bool send_move_packet(PlayerInfo pi);
	bool send_chat_packet(std::wstring name, std::wstring chat, unsigned int id);
	bool send_update_item_packet(unsigned short id, unsigned short num); // 해당 아이템이 num개로 업데이트
	bool send_update_gold(int sc_gold_offset);
	bool send_get_quest_packet(unsigned short gid, unsigned short n);
	bool send_remove_quest_packet(unsigned short gid, unsigned short n);
	bool send_room_setup_packet();
	bool send_room_leave_packet();
	bool send_time_sync_packet();
	bool send_voice_chat_packet();
	bool send_update_party_packet();
	bool send_invite_call_packet(std::string& id, std::wstring& name);
	bool send_reject_call_packet(std::string& id);
	bool send_join_success_packet(std::string& id, std::wstring& name);

	// 핸들러 함수
	void handle_party_packet(CS_UPDATE_PARTY_PACKET& pkt);

public:

	Player() : socket(0), state(NONE) {}
	Player(SOCKET s, int id) : socket(s) { pinfo.id = id; }
	~Player() {}

	void init_player(SOCKET s, int id)
	{
		socket = s;
		pinfo.id = id;
	}

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

	int Get_id() { return pinfo.id; }
};

