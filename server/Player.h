#pragma once

// none: �÷������� �ƴ�, connecting: ���� Login success�� ������ ��, playing: ������
enum STATES { NONE = 0, CONNECTING = 1, PLAYING = 2 };

class Player
{
	EXT_OVER over;
	SOCKET socket;
	vector<char> packet_data; // deque�� ����ұ�?
	int id = 0;

	string name;
	PlayerInfo pinfo;

	bool send_login_info_packet(bool b, PlayerInfo pi);
	bool send_spawn_packet(PlayerInfo pi);
	bool send_despawn_packet(int id);
	bool send_move_packet(PlayerInfo pi);
	bool send_chat_packet(string name, wstring chat);
	bool send_update_item_packet(unsigned short id, unsigned short num); // �ش� �������� num���� ������Ʈ

	unordered_map<unsigned short, unsigned short> player_item;

public:
	STATES state;

	Player() : socket(0), state(NONE) {}
	Player(SOCKET s, int id) : socket(s), id(id) {}
	~Player() {}

	void send(void* packet);
	void recv();
	void handle_packet(char* packet, unsigned short length);

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

	// �߰����� ����� ���� getter�� setter�� �߰��� �� �ֽ��ϴ�.
	SOCKET get_socket() const { return socket; }
	void set_socket(SOCKET sock) { socket = sock; }

	STATES get_state() const { return state; }
	void set_state(STATES st) { state = st; }
};

