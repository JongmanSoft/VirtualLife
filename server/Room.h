#pragma once
// #include "stdafx.h"
#include <vector>
#include <memory>
#include <iostream>
#include "protocol.h"

class Room
{
private:
	// vector<Object*> objs; // 현재 집에 깔린 가구들
	std::vector<std::shared_ptr<Object>> objs;
public:
	Room() {};
	~Room() {};

	void AddObject(const Object& obj);
	void packet_setup(SC_ROOM_SETUP_PACKET& pkt);

};

