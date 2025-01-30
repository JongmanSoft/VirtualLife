#pragma once

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

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")
using namespace std;

#include "protocol.h"



int setid();
void server_error(const char* msg);

constexpr int BUFSIZE = 256;

enum class TASK_TYPE
{
    // OV_TYPE
    ACCEPT,
    RECV,
    SEND,
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