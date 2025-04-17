#pragma once
#include "stdafx.h"

class DBManager {
public:
    static void Init();
    static sql::Connection* GetConnection();

    static bool checkLogin(const std::string& id, const std::string& pw);


private:
    static sql::mysql::MySQL_Driver* g_driver;
    static thread_local std::unique_ptr<sql::Connection> t_conn;
    static thread_local std::unique_ptr<sql::Statement> t_stmt;
};