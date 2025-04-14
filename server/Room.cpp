#include "Room.h"


void Room::AddObject(const Object& obj)
{
    if (objs.size() >= MAX_BUILD_ITEM) return;

    objs.push_back(std::make_shared<Object>(obj));

    std::cout << "Room에 오브젝트 추가됨: ID " << obj.item_id << " 위치 (" << obj.x << ", " << obj.y << ", " << obj.z << ")\n";
}

void Room::packet_setup(SC_ROOM_SETUP& pkt)
{
	// todo
}
