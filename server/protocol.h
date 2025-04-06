// 최대 길이 설정
constexpr int PORT_NUM = 7777;
constexpr int M_ID_SIZE = 20;
constexpr int CHAT_SIZE = 100; 

constexpr short MAX_PLAYER = 1000;

constexpr int ITEM_SIZE = 12; // 아이템 종류 수: 수정 필요

enum STATE : char { IDLE, WALK, RUN, JUMP };
struct PlayerInfo
{
	unsigned int id;
	float x;
	float y;
	float z;
	float yaw; // 회전값
	STATE st; // 애니메이션 동기화용?
};

struct AdditionalInfo
{
	unsigned short job;
	unsigned short age;
	short hp;
	short mp;
	int gold;
};

struct Customizing
{
	float skin; // 피부색
	unsigned short shirt;
	unsigned short pants;
	unsigned short shoes;
	//눈정보 
	float R_eye_color_hue;
	float R_eye_color_sat;
	float L_eye_color_hue;
	float L_eye_color_sat;
	float eye_scale;
	float pupil_scale;
	//헤어정보
	unsigned short hair;
	float hair_color_R;
	float hair_color_G;
	float hair_color_B;

	//얼굴 커스텀
	//눈
	float eye_width;
	float eye_thick;
	float eye_slope;
	//코
	float nose_width;
	float nose_height;
	//입
	float mouse_width;
	float mouse_thick;
	float mouse_slope;
	//얼굴형
	float chin;
	float jaw;
	float heavy;
	float face_width;
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
	CS_GET_ITEM,
	CS_UPDATE_CUSTOM,
	CS_ENTER_GAME, // 클라의 게임접속.
	CS_GET_QUEST, // 클라가 퀘스트 획득 요청
	CS_UPDATE_GOLD,

	// server to client
	SC_LOGININFO,
	SC_ENTER_GAME,
	SC_SPAWN,
	SC_DESPAWN,
	SC_LEAVE,
	SC_CHAT,
	SC_MOVEP,
	SC_UPDATE_ITEM,
	SC_DROP_ITEM,
	SC_UPDATE_CUSTOM,
	SC_UPDATE_GOLD
};

constexpr int HEADER_SIZE = sizeof(PACKETID) + sizeof(unsigned short);
// client to server
#pragma pack (push, 1)
struct CS_LOGIN_PACKET { // 로그인 요청.
	unsigned short size;
	PACKETID	type;
	char	id[M_ID_SIZE];
};

struct CS_ENTER_GAME_PACKET { // 게임 접속
	unsigned short size;
	PACKETID	type;
	wchar_t name[M_ID_SIZE];
};

struct CS_LEAVE_PACKET {
	unsigned short size;
	PACKETID	type;
};

struct CS_CHAT_PACKET {
	unsigned short size;	
	PACKETID	type;
	wchar_t	name[M_ID_SIZE]; // 대충.. 임시로..
	wchar_t	msg[CHAT_SIZE];
};

struct CS_MOVE_PACKET {
	unsigned short size;
	PACKETID type;
	PlayerInfo pl;
};

struct CS_GET_ITEM_PACKET { // 아이템 획득 -> 무조건 한개. 로 해도 될까?
	unsigned short size;
	PACKETID type;
	unsigned short id;
	unsigned short num;
};

struct CS_UPDATE_CUSTOM_PACKET {
	unsigned short size;
	PACKETID type;
	Customizing c;
};

struct CS_UPDATE_GOLD_PACKET
{
	unsigned short size;
	PACKETID type;
	int gold_offset; //증감수치, 500원 썼으면 -500. 500원받았으면 +500
};

// server to client
struct SC_LOGIN_INFO_PACKET {
	unsigned short size;
	PACKETID type;
	bool success;
};

struct SC_ENTER_GAME_PACKET { // 클라에게 내 캐릭터의 정보 제공
	unsigned short size;
	PACKETID type;
	wchar_t name[M_ID_SIZE]; // 내이름
	PlayerInfo player;
	unsigned short items[ITEM_SIZE];
	Customizing custom; // 내 캐릭터의 커스터마이징 정보
	AdditionalInfo addinfo;
};

struct SC_SPAWN_PACKET {
	unsigned short size;
	PACKETID	type;
	PlayerInfo pl;
	Customizing c;
};

struct SC_DESPAWN_PACKET {
	unsigned short size;
	PACKETID	type;
	unsigned int id;
};

struct SC_CHAT_PACKET {
	unsigned short size;
	PACKETID	type;
	wchar_t	name[M_ID_SIZE];
	wchar_t	msg[CHAT_SIZE];
};

struct SC_MOVE_PACKET {
	unsigned short size;
	PACKETID type;
	PlayerInfo pl;
};

struct SC_UPDATE_ITEM_PACKET {
	unsigned short size;
	PACKETID type;
	unsigned short id;
	unsigned short num;
};

struct SC_UPDATE_CUSTOM_PACKET { // 이미 전송한 플레이어의 커스터마이징 업뎃
	unsigned short size;
	PACKETID type;
	unsigned short id;
	Customizing c;
};

struct SC_UPDATE_GOLD_PACKET {
	unsigned short size;
	PACKETID type;
	int gold; //클라가 준 증감수치를 더한 최종골드
};
#pragma pack (pop)