#include "Kid.h"
#include <cstring> 

Kid::Kid(unsigned int id, unsigned int preg_id, unsigned int spouse_id, const wchar_t* name,
	const Customizing& cus, char personality, const wchar_t* msg)
	: id(id), preg_id(preg_id), spouse_id(spouse_id), customizing(cus), personality(personality)
{
	this->hello_msg = hello_msg;
	this->name = name; // 이름 복사

}

Kid::Kid(const CS_ADD_KID_PACKET& pkt)
	: id(pkt.preg_id), preg_id(pkt.preg_id), spouse_id(pkt.spouse_id), customizing(pkt.c), personality(pkt.personality) 
{
	hello_msg = pkt.hello_msg;
	name = pkt.name; // 이름 복사
	x = pkt.x;
	y = pkt.y;	
	z = pkt.z;
	yaw = pkt.yaw; 
}
