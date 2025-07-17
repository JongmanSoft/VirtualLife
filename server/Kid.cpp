#include "Kid.h"
#include <cstring> 

Kid::Kid(unsigned int id, unsigned int preg_id, unsigned int spouse_id, const wchar_t* name,
	const Customizing& cus, char personality, const wchar_t* msg)
	: id(id), preg_id(preg_id), spouse_id(spouse_id), customizing(cus), personality(personality)
{
	wcsncpy_s(hello_msg, msg, CHAT_SIZE - 1);
	hello_msg[CHAT_SIZE - 1] = L'\0'; // 안전하게 널 종료
	wcsncpy_s(_name, name, M_ID_SIZE - 1);
	_name[M_ID_SIZE - 1] = L'\0'; // 안전하게 널 종료	

}

Kid::Kid(const CS_ADD_KID_PACKET& pkt)
	: id(pkt.preg_id), preg_id(pkt.preg_id), spouse_id(pkt.spouse_id), customizing(pkt.c), personality(pkt.personality) 
{
	wcsncpy_s(hello_msg, pkt.hello_msg, CHAT_SIZE - 1);
	hello_msg[CHAT_SIZE - 1] = L'\0'; // 안전하게 널 종료
	wcsncpy_s(_name, pkt.name, M_ID_SIZE - 1);
	name[M_ID_SIZE - 1] = L'\0'; // 안전하게 널 종료
}
