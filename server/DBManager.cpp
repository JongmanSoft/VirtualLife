#include "DBManager.h"

sql::mysql::MySQL_Driver* DBManager::g_driver = nullptr;
thread_local std::unique_ptr<sql::Connection> DBManager::t_conn = nullptr;

void DBManager::Init() 
{
    if (!g_driver) {
        g_driver = sql::mysql::get_driver_instance();
    }
}

sql::Connection* DBManager::GetConnection() 
{
    if (!t_conn) {
        t_conn = std::unique_ptr<sql::Connection>(
            g_driver->connect("tcp://127.0.0.1:3306", "root", "12345678"));
        t_conn->setSchema("VL_DB");
    }
    return t_conn.get();
}

bool DBManager::checkLogin(const std::string& id, const std::string& pw)
{
    try {
        sql::Connection* conn = GetConnection();

        // 1. ID 존재 여부 + PW 조회
        std::unique_ptr<sql::PreparedStatement> stmt(
            conn->prepareStatement("SELECT PW FROM LOGIN_INFO WHERE ID=?"));
        stmt->setString(1, id);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());

        if (res->next()) {
            // ID 존재 → 저장된 PW와 비교
            std::string db_pw = res->getString("PW");
            return db_pw == pw;
        }
        else {
            // ID 존재하지 않음 → INSERT
            std::unique_ptr<sql::PreparedStatement> insertStmt(
                conn->prepareStatement("INSERT INTO LOGIN_INFO (ID, PW, IS_PLAYING) VALUES (?, ?, FALSE)"));
            insertStmt->setString(1, id);
            insertStmt->setString(2, pw);
            insertStmt->executeUpdate();
            return true;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "[DB Error] " << e.what() << std::endl;
        return false;
    }
}