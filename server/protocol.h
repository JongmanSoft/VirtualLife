#pragma once

#define __PROTOCOL_H__

#pragma pack (push, 1)

// 최대 길이 설정
constexpr int PORT_NUM = 7777;
constexpr int M_ID_SIZE = 20;
constexpr int CHAT_SIZE = 100; 
constexpr short MAX_PLAYER = 1000;
constexpr unsigned short MAX_BUILD_ITEM = 200;
constexpr int ITEM_SIZE = 12; // 아이템 종류 수: 수정 필요
constexpr int QUEST_MAX = 10; // 퀘스트 개수 ?
constexpr int MAX_PARTY_MEMBER = 10; // 파티 최대 인원 수

// 파티 관련 패킷 응답
enum PARTY_REQUEST : char
{
	PARTY_REQUEST_INVITE = 0,
	PARTY_REQUEST_INVITE_REJECT,
	PARTY_REQUEST_INVITE_ACCEPT,
	PARTY_UPDATE
};

enum STATE : char { IDLE, WALK, RUN, JUMP, MINE, FISH, SEED, HOME };
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
	int gold = 500;
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
	//아이브로우,마스카라
	unsigned short eyebrows;
	unsigned short glasses;
};

struct Object // 건축 오브젝트
{
	unsigned short item_id;
	float x;
	float y;
	float z;
	float yaw; // 회전값
	float scale; // 크기
};

// Packet ID
enum PACKETID : char
{
	// client to server
	CS_LOGIN = 0,
	CS_SPAWN,
	CS_LEAVE,
	CS_CHAT,
	CS_MOVEP,
	CS_GET_ITEM,
	CS_UPDATE_CUSTOM,
	CS_ENTER_GAME, // 클라의 게임접속.
	CS_UPDATE_GOLD,
	CS_GET_QUEST, // 클라가 퀘스트 획득 요청
	CS_REMOVE_QUEST, //클라가 퀘스트 삭제요청 (완료시)
	CS_NPC_CHAT,
	CS_PLACE_BUILD,
	CS_ROOM_ENTER,
	CS_REMOVE_BUILD,
	CS_UPDATE_BUILD,
	CS_ROOM_LEAVE,
	CS_TIME_SYNC,
	CS_VOICE_CHAT,
	CS_UPDATE_PARTY, // TYPE: JOIN(0), LEAVE(1), CREATE(2)

	// server to client
	SC_LOGININFO,
	SC_ENTER_GAME,
	SC_SPAWN,
	SC_DESPAWN,
	SC_LEAVE,
	SC_CHAT,
	SC_MOVEP,
	SC_UPDATE_ITEM,
	SC_UPDATE_CUSTOM,
	SC_UPDATE_GOLD,
	SC_GET_QUEST,
	SC_REMOVE_QUEST,
	SC_NPC_RESPONSE,
	SC_ROOM_SETUP,
	SC_ROOM_LEAVE,
	SC_TIME_SYNC,
	SC_VOICE_CHAT,
	SC_UPDATE_PARTY, 
	SC_RESULT_PARTY
};

constexpr int HEADER_SIZE = sizeof(PACKETID) + sizeof(unsigned short);
// client to server

// 로그인 -------------------------------
struct CS_LOGIN_PACKET { // 로그인 요청
	unsigned short size;
	PACKETID	type;
	char	id[M_ID_SIZE];
	char	pw[M_ID_SIZE];
};

struct SC_LOGIN_INFO_PACKET {
	unsigned short size;
	PACKETID type;
	bool success;
	bool is_new;
};

// 게임 접속 ------------------------------
struct CS_ENTER_GAME_PACKET { // 게임 접속
	unsigned short size;
	PACKETID	type;
	wchar_t name[M_ID_SIZE];
};

struct SC_ENTER_GAME_PACKET { // 클라에게 내 캐릭터의 정보 제공
	unsigned short size;
	PACKETID type;
	wchar_t name[M_ID_SIZE]; // 내이름
	PlayerInfo player;
	unsigned short items[ITEM_SIZE];
	float time; 
	Customizing custom; // 내 캐릭터의 커스터마이징 정보
	AdditionalInfo addinfo;
	short giver_id[QUEST_MAX];
	short num[QUEST_MAX]; // 퀘스트 번호
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

// 게임 종료 ---------------------------
struct CS_LEAVE_PACKET {
	unsigned short size;
	PACKETID	type;
};

// 채팅 --------------------------------
struct CS_CHAT_PACKET {
	unsigned short size;	
	PACKETID	type;
	unsigned int from_id; //보낸넘 아이디
	wchar_t	name[M_ID_SIZE]; // 대충.. 임시로..
	wchar_t	msg[CHAT_SIZE];
};

struct SC_CHAT_PACKET {
	unsigned short size;
	PACKETID	type;
	unsigned int from_id;
	wchar_t	name[M_ID_SIZE];
	wchar_t	msg[CHAT_SIZE];
};

// 이동 ---------------------------------
struct CS_MOVE_PACKET {
	unsigned short size;
	PACKETID type;
	PlayerInfo pl;
};

struct SC_MOVE_PACKET {
	unsigned short size;
	PACKETID type;
	PlayerInfo pl;
};

// 아이템 관련 ---------------------------
struct CS_GET_ITEM_PACKET {
	unsigned short size;
	PACKETID type;
	unsigned short id;
	short num;
};

struct SC_UPDATE_ITEM_PACKET {
	unsigned short size;
	PACKETID type;
	unsigned short id;
	short num;
};

// 커스터마이징 값 관련 -----------------
struct CS_UPDATE_CUSTOM_PACKET {
	unsigned short size;
	PACKETID type;
	Customizing c;
};

struct SC_UPDATE_CUSTOM_PACKET { // 이미 전송한 플레이어의 커스터마이징 업뎃
	unsigned short size;
	PACKETID type;
	unsigned short id;
	Customizing c;
};

// 골드 관련 ----------------------------
struct CS_UPDATE_GOLD_PACKET
{
	unsigned short size;
	PACKETID type;
	int gold_offset; //증감수치, 500원 썼으면 -500. 500원받았으면 +500
};

struct SC_UPDATE_GOLD_PACKET {
	unsigned short size;
	PACKETID type;
	int gold; //클라가 준 증감수치를 더한 최종골드
};

// 퀘스트 관련 --------------------------
struct CS_UPDATE_QUEST_PACKET {
	unsigned short size;
	PACKETID type;
	short giver_id;
	short num; // 퀘스트 번호
};

struct SC_UPDATE_QUEST_PACKET {
	unsigned short size;
	PACKETID type;
	short giver_id;
	short num; // 퀘스트 번호
};

// 건축 관련 ----------------------------
struct CS_PLACE_BUILD_PACKET {
	unsigned short size;
	PACKETID type;
	Object build;
};

struct CS_REMOVE_BUILD_PACKET {
	unsigned short size;
	PACKETID type;
	float x, y, z;
};

struct CS_UPDATE_BUILD_PACKET {
	unsigned short size;
	PACKETID type;
	float old_x, old_y, old_z;
	float new_x, new_y, new_z;
	float new_yaw;
};

// 집 입장 -----------------------------
struct CS_ROOM_ENTER_PACKET {
	unsigned short size;
	PACKETID type;
};

struct CS_ROOM_LEAVE_PACKET {
	unsigned short size;
	PACKETID type;
};

struct SC_ROOM_SETUP_PACKET {
	unsigned short size;
	PACKETID type;
	unsigned short id; // 현재 접속한 집 주인의 아이디
	unsigned short count; // 실제 오브젝트 개수
	Object objs[MAX_BUILD_ITEM]; // 오브젝트들
};

struct SC_ROOM_LEAVE_PACKET {
	unsigned short size;
	PACKETID type;
	float x, y, z;
	float time;
};

// 파티 관련 --------------------------- ( 하는중 )

struct CS_UPDATE_PARTY_PACKET {
	unsigned short size;
	PACKETID type; // CS_UPDATE_PARTY
	PARTY_REQUEST act_type; 
	char id[M_ID_SIZE]; // 입장할 파티의 유저 아이디
};

struct SC_UPDATE_PARTY_PACKET {
	unsigned short size;
	PACKETID type; // SC_UPDATE_PARTY
	PARTY_REQUEST act_type; // JOIN_SUCCESS(0), JOIN_FAIL(1), CREATE_SUCCESS(2), CREATE_FAIL(3), INVITE(4), KICK(5), MEMBER_UPDATE(6)
	unsigned char member_count;
	unsigned int membersID[MAX_PARTY_MEMBER];
};

struct SC_RESULT_PARTY_PACKET {
	unsigned short size;
	PACKETID type; // CS_UPDATE_PARTY
	PARTY_REQUEST act_type;
	char id[M_ID_SIZE]; // 누가 초대를 보냈는가 OR 거절했는지, 수락했는지
};

// 미완 ------------------------------
struct CS_NPC_CHAT_PACKET {
	unsigned short size;
	PACKETID type;
	unsigned short npc_id;
	wchar_t	msg[CHAT_SIZE];
};

struct CS_TIME_SYNC_PACKET {
	unsigned short size;
	PACKETID type;
	float ping;
};

struct CS_VOICE_CHAT_PACKET {
	unsigned short size;
	PACKETID type; // CS_VOICE_CHAT
	unsigned int from_id;
	unsigned short data_len;
	char data[512]; // Opus 압축 데이터
};

struct SC_TIME_SYNC_PACKET {
	unsigned short size;
	PACKETID type;
	float curtime;
	float time;
};

struct SC_VOICE_CHAT_PACKET {
	unsigned short size;
	PACKETID type;
	unsigned int from_id;
	unsigned short data_len;
	char data[512]; // Opus 압축 데이터
};

struct SC_NPC_RESPONSE_PACKET {
	unsigned short size;
	PACKETID type;
	unsigned short npc_id;
	wchar_t	msg[CHAT_SIZE];
};

#pragma pack (pop)