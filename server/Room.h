#pragma once
// #include "stdafx.h"
#include <vector>
#include <memory>
#include <iostream>
#include "protocol.h"
#include <mutex>

class Player;

class Room
{
private:
	// vector<Object*> objs; // 현재 집에 깔린 가구들
	std::vector<std::shared_ptr<Object>> objs;
	std::vector<Player*> players; // 방에 있는 플레이어들
	std::mutex mtx; // 동기화용 mutex

public:
	Room() {};
	~Room() {};

	void AddPlayer(Player* player);
	void RemovePlyer(int id);
	void AddObject(const Object& obj);
	void packet_setup(SC_ROOM_SETUP_PACKET& pkt);
	void RemoveObjectByPosition(float x, float y, float z);
	void UpdateObjectTransform(float old_x, float old_y, float old_z, float new_x, float new_y, float new_z, float new_yaw);

	void SaveToDB(const std::string& userID);
	void LoadFromDB(const std::string& userID);
};

