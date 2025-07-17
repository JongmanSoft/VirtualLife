#pragma once
#include "stdafx.h"

class DBManager {
public:
    static bool DB_ON;
    static void Init();
    static sql::Connection* GetConnection();

    // 디비에 아이디 PW 저장
    static bool checkLogin(const std::string& id, const std::string& pw, bool& is_new);

    // 커마값 디비에 업데이트
    static void SaveCustomizing(const std::string& userID, const Customizing& data);
    static bool LoadCustomizing(const std::string& userID, Customizing& outData);
    static void SaveDefCustomizing(const std::string& id);
    
    // pinfo
    static void SaveDefPInfo(const std::string& userID, const PlayerInfo& data);
    static void SavePInfo(const std::string& userID, const PlayerInfo& data, const std::string& name);
    static void SavePInfo(const std::string& userID, const PlayerInfo& data);
    static bool LoadPInfo(const std::string& userID, PlayerInfo& outInfo, std::wstring& outName);
	static void SavePname(const std::string& userID, const std::wstring& name);

    static void SaveGold(const std::string& userID, const int gold);
    static int LoadGold(const std::string& userID);

    // inventory
    static void SaveItem(const std::string& userID, int itemCode, int itemCount);
    static bool LoadItem(const std::string& userID, std::unordered_map<unsigned short, unsigned short>& outData);

    // Room
    static void SaveRoomObject(const std::string& userID, const Object& object);
    static void UpdateRoomObject(const std::string& userID, float oldX, float oldY, float oldZ, float newX, float newY, float newZ, float newYaw);
    static bool LoadRoomObjects(const std::string& userID, std::vector<Object>& outObjects);
    static void DeleteRoomObject(const std::string& userID, float posX, float posY, float posZ);
	static void SaveRoomObjects(const std::string& userID, const std::vector<Object>& objects); // 전체 저장하는 함수
    static void DeleteRoomObjects(const std::string& userID); // 전체 삭제하는 함수

    // Quest
	static bool LoadQuest(const std::string& userID, std::vector<Quest>& outData);
	static void SaveQuest(const std::string& userID, Quest& data);
    static void DeleteQuest(const std::string& userID, Quest& data);

    //Kid
    static void SaveKidInfo(const Kid& kid);
    static bool LoadKidInfo(unsigned int id, Kid& outKid);

    // 초기초기화
    static bool LoadAllRoomsFromDB();
    static bool LoadAllKidsFromDB();

private:
    static sql::mysql::MySQL_Driver* g_driver;
    static thread_local std::unique_ptr<sql::Connection> t_conn;
    static thread_local std::unique_ptr<sql::Statement> t_stmt;
};

