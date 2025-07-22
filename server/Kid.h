#pragma once
#include "protocol.h"

#include <string>
class Kid 
{
public:
	unsigned int id; 
	unsigned int preg_id;
	unsigned int spouse_id; 
	Customizing customizing; 
	char personality; 
	float x = 0, y = 0, z = 0, yaw = 0; 
	bool is_kid = false;
	std::wstring hello_msg = L"";
	std::wstring name = L""; 

	Kid(unsigned int id, unsigned int preg_id, unsigned int spouse_id, const wchar_t* name,
		const Customizing& cus, char personality, const wchar_t* msg);
	Kid(const CS_ADD_KID_PACKET& pkt);
	Kid() : id(0), preg_id(0), spouse_id(0), personality(0) {
		hello_msg = L'\0';
		customizing = Customizing(); 
		x = 0.0f; y = 0.0f; z = 0.0f; yaw = 0.0f; 
		name = L'\0'; 
	}
};

