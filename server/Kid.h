#pragma once
#include "protocol.h"

#include <string>
class Kid 
{
public:
	unsigned int id; // 아이디
	unsigned int preg_id; // 부모 아이디
	unsigned int spouse_id; // 배우자 아이디
	Customizing customizing; // 커스터마이징 정보
	char personality; // 성격
	wchar_t _name[M_ID_SIZE]; // 이름
	wchar_t hello_msg[CHAT_SIZE]; // 인사말
	float x = 0, y = 0, z = 0, yaw = 0; // 위치 정보
	bool is_kid = false;
	std::wstring name = L""; // 이름

	Kid(unsigned int id, unsigned int preg_id, unsigned int spouse_id, const wchar_t* name,
		const Customizing& cus, char personality, const wchar_t* msg);
	Kid(const CS_ADD_KID_PACKET& pkt);
	Kid() : id(0), preg_id(0), spouse_id(0), personality(0) {
		hello_msg[0] = L'\0'; // 초기화
		customizing = Customizing(); // 기본 커스터마이징 정보 초기화
		x = 0.0f; y = 0.0f; z = 0.0f; yaw = 0.0f; // 위치 초기화
		_name[0] = L'\0'; // 이름 초기화
	}
};

