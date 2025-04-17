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

constexpr int BUFSIZE = 256;

enum class TASK_TYPE
{
    // OV_TYPE
    ACCEPT,
    RECV,
    SEND,
    DB_UPDATE // 플레이어 정보 업데이트
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