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
	void RemoveObjectByPosition(float x, float y, float z);
	void UpdateObjectTransform(float old_x, float old_y, float old_z, float new_x, float new_y, float new_z, float new_yaw);

	void SaveToDB(const std::string& userID);
	void LoadFromDB(const std::string& userID);
};

