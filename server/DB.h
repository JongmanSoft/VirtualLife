#pragma once
#include "stdafx.h"
#include "db/include/jdbc/mysql_driver.h"
#include "db/include/jdbc/mysql_connection.h"
#include "db/include/jdbc/mysql_error.h"
#include "db/include/jdbc/cppconn/statement.h"
#include "db/include/jdbc/cppconn/resultset.h"


// DB 연결 전역 함수
std::unique_ptr<sql::Connection> db_connect(const std::string& host, const std::string& user, const std::string& pass, const std::string& db) {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::string connection_str = "tcp://" + host + ":3306";  // 주소 및 포트
        std::unique_ptr<sql::Connection> conn(driver->connect(connection_str, user, pass));
        conn->setSchema(db);  // 데이터베이스 선택
        std::cout << "DB connected successfully!" << std::endl;
        return conn;
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error connecting to database: " << e.what() << std::endl;
        return nullptr;
    }
}

// DB 쿼리 실행 전역 함수
void execute_query(std::unique_ptr<sql::Connection>& conn, const std::string& sql_query) {
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(sql_query));

        while (res->next()) {
            std::cout << "Column 1: " << res->getString(1) << std::endl;
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Query failed: " << e.what() << std::endl;
    }
}