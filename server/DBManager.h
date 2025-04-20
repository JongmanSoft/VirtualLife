#pragma once
#include "stdafx.h"

class DBManager {
public:
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

private:
    static sql::mysql::MySQL_Driver* g_driver;
    static thread_local std::unique_ptr<sql::Connection> t_conn;
    static thread_local std::unique_ptr<sql::Statement> t_stmt;
};