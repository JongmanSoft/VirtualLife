#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <random>
#include <thread>
#include <unordered_set>
#include <concurrent_priority_queue.h>
#include <sqlext.h> 
#include <chrono>
#include <array>
#include <mutex>
#include <string>
#include <sstream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <codecvt>
#include <locale>

// db때문에 04.15 추가
#include <memory>
#include <stdexcept>
#include "db/include/jdbc/mysql_driver.h"
#include "db/include/jdbc/mysql_connection.h"
#include "db/include/jdbc/mysql_error.h"
#include "db/include/jdbc/cppconn/statement.h"
#include "db/include/jdbc/cppconn/resultset.h"
#include "db/include/jdbc/cppconn/prepared_statement.h"

#ifdef _DEBUG
#pragma comment(lib, "db\\lib64\\vs14\\mysqlcppconn.lib")
#else
#pragma comment(lib, "db\\lib64\\vs14\\mysqlcppconn.lib")
#endif

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

#include "protocol.h"
#include "Utility.h"

int setid();
void server_error(const char* msg);

constexpr int BUFSIZE = 8192;
constexpr int DB_POS_UPDATE_TIME = 60000; // 여기 수정해서 디비 업데이트 텀 수정 가능
constexpr int DB_INVENTORY_UPDATE_TIME = 30000;

enum class TASK_TYPE
{
    // OV_TYPE
    ACCEPT,
    RECV,
    SEND,
    DB_POS_UPDATE, // 플레이어 정보 업데이트
    TIME_UPDATE, // 시간 업데이트
    DB_INVENTORY_UPDATE
};

class EVENT
{
public:
    TASK_TYPE evt_type;
    int to_id; // 누구에게 -> 이거 필요할까
    int from_id; // 누가
    std::chrono::system_clock::time_point do_time;

    EVENT() {}

    void setup(TASK_TYPE evt, int s_time, int from = -1, int to = -1)// time->ms
    {
        evt_type = evt;
        from_id = from;
        to_id = to;
        do_time = std::chrono::system_clock::now() + std::chrono::milliseconds(s_time);
    }

    std::chrono::system_clock::time_point& GETTIME() { return do_time; }

    bool operator<(const EVENT& other) const
    {
        return do_time > other.do_time;
    }
};

class EXT_OVER // overlapped, packet size, type
{
public:
    WSAOVERLAPPED over;
    WSABUF wsabuf;
    char wb_buf[BUFSIZE];
    TASK_TYPE ov;
    int from;
    int to;

    EXT_OVER() // recv
    {
        wsabuf.len = BUFSIZE;
        wsabuf.buf = wb_buf;
        ov = TASK_TYPE::RECV;
        ZeroMemory(&over, sizeof(over));
    }

    void setup_send(char* pk, int len) // send
    {
        wsabuf.len = len;
        wsabuf.buf = wb_buf;
        ZeroMemory(&over, sizeof(over));
        ov = TASK_TYPE::SEND;
        memcpy(wb_buf, pk, len);
    }
};

#include "Player.h"
extern std::array<Player, MAX_PLAYER> players;
extern std::mutex players_mutex;
extern float f_time;
extern std::chrono::high_resolution_clock::time_point startTime;