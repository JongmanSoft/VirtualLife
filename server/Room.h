#pragma once
#include "stdafx.h"

class Room
{
private:
	vector<Object*> objs; // 현재 집에 깔린 가구들

public:
	// 건물 추가
	void AddObject(const Object& obj);

	void packet_setup(SC_ROOM_SETUP& pkt);

};

