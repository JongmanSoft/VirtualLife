// 최대 길이 설정
constexpr int PORT_NUM = 7777;
constexpr int M_NAME_SIZE = 20;
constexpr int CHAT_SIZE = 100; 

constexpr short MAX_PLAYER = 1000;

struct PlayerInfo
{
	unsigned int id;
	float x;
	float y;
	float z;
	float yaw; // 회전값
};

// Packet ID
enum PACKETID : char
{
	// client to server
	CS_LOGIN = 0,
	CS_SPAWN = 1,
	CS_LEAVE = 2,
	CS_CHAT,
	CS_MOVEP,

	// server to client
	SC_LOGININFO,
	SC_SPAWN,
	SC_DESPAWN,
	SC_LEAVE,
	SC_CHAT,
	SC_MOVEP
};

constexpr int HEADER_SIZE = sizeof(PACKETID) + sizeof(unsigned short);
// client to server
#pragma pack (push, 1)
struct CS_LOGIN_PACKET { // 로그인 요청.
	unsigned short size;
	PACKETID	type;
	char	name[M_NAME_SIZE];
};

struct CS_LEAVE_PACKET {
	unsigned short size;
	PACKETID	type;
};

struct CS_CHAT_PACKET {
	unsigned short size;	
	PACKETID	type;
	char	name[M_NAME_SIZE];
	wchar_t	msg[CHAT_SIZE];
};

struct CS_MOVE_PACKET {
	unsigned short size;
	PACKETID type;
	PlayerInfo pl;
};

// server to client
struct SC_LOGIN_INFO_PACKET {
	unsigned short size;
	PACKETID type;
	bool success;
	PlayerInfo player;
};

struct SC_SPAWN_PACKET {
	unsigned short size;
	PACKETID	type;
	PlayerInfo pl;
};

struct SC_DESPAWN_PACKET {
	unsigned short size;
	PACKETID	type;
	unsigned int id;
};

struct SC_CHAT_PACKET {
	unsigned short size;
	PACKETID	type;
	char	name[M_NAME_SIZE];
	wchar_t	msg[CHAT_SIZE];
};

struct SC_MOVE_PACKET {
	unsigned short size;
	PACKETID type;
	PlayerInfo pl;
};
#pragma pack (pop)