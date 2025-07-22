#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <unordered_set>
#include <mutex>
#include <atomic>
#include <chrono>
#include <queue>
#include <array>
#include <memory>
#include "../server/protocol.h"

using namespace std;
using namespace chrono;

const static int MAX_TEST = 5000;
constexpr int MAX_CLIENTS = 5000;
const static int INVALID_ID = -1;
const static int MAX_PACKET_SIZE = 255;
const static int MAX_BUFF_SIZE = 255;
#pragma comment (lib, "ws2_32.lib")

HANDLE g_hiocp;

enum OPTYPE { OP_SEND, OP_RECV };

high_resolution_clock::time_point last_connect_time;

struct OverlappedEx {
    WSAOVERLAPPED over;
    WSABUF wsabuf;
    unsigned char IOCP_buf[MAX_BUFF_SIZE];
    OPTYPE event_type;
};

struct CLIENT {
    SOCKET client_socket;
    atomic_bool connected;
    OverlappedEx recv_over;
    unsigned char packet_buf[MAX_PACKET_SIZE];
    int prev_packet_data;
    int curr_packet_size;
    high_resolution_clock::time_point last_send_time;
    vector<double> ping_history;
    mutex ping_mutex;
};

array<CLIENT, MAX_CLIENTS> g_clients;
atomic_int num_connections;
atomic_int active_clients;

int64_t get_chrono_timestamp() {
    return duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

void error_display(const char* msg, int err_no) {
    cerr << msg << ", Error Code: " << err_no << endl;
}

void DisconnectClient(int ci) {
    if (g_clients[ci].connected.exchange(false)) {
        closesocket(g_clients[ci].client_socket);
        active_clients--;
    }
}

void SendPacket(int ci, void* packet) {
    int psize = reinterpret_cast<unsigned char*>(packet)[0];
    OverlappedEx* over = new OverlappedEx;
    over->event_type = OP_SEND;
    memcpy(over->IOCP_buf, packet, psize);
    ZeroMemory(&over->over, sizeof(over->over));
    over->wsabuf.buf = reinterpret_cast<CHAR*>(over->IOCP_buf);
    over->wsabuf.len = psize;

    int ret = WSASend(g_clients[ci].client_socket, &over->wsabuf, 1, NULL, 0, &over->over, NULL);
    if (ret != 0 && WSAGetLastError() != WSA_IO_PENDING) {
        error_display("SendPacket", WSAGetLastError());
        delete over;
    }
}

void ProcessPacket(int ci, unsigned char* packet) {
    switch (packet[1]) {
    case SC_TEST_MOVE: {
        SC_TEST_MOVE_PACKET* resp = reinterpret_cast<SC_TEST_MOVE_PACKET*>(packet);
        int64_t now = get_chrono_timestamp();
        int64_t rtt_us = now - resp->server_send_time;
        double ping_ms = static_cast<double>(rtt_us) / 1000.0;

        if (!isnan(ping_ms) && ping_ms >= 0.0 && ping_ms < 10000.0) {
            lock_guard<mutex> lg(g_clients[ci].ping_mutex);
            g_clients[ci].ping_history.push_back(ping_ms);
            if (g_clients[ci].ping_history.size() > 100)
                g_clients[ci].ping_history.erase(g_clients[ci].ping_history.begin());
        }

        {
            lock_guard<mutex> lg(g_clients[ci].ping_mutex);
            if (g_clients[ci].ping_history.size() >= 5) {
                double avg = 0;
                for (auto& v : g_clients[ci].ping_history) avg += v;
                avg /= g_clients[ci].ping_history.size();
                if (avg > 500.0) {
                    CS_LEAVE_PACKET leave{};
                    leave.size = sizeof(leave);
                    leave.type = CS_LEAVE;
                    SendPacket(ci, &leave);
                    DisconnectClient(ci);
                    return;
                }
            }
        }
        break;
    }
    }
}

void WorkerThread() {
    while (true) {
        DWORD io_size;
        ULONG_PTR key;
        OverlappedEx* over;
        BOOL ret = GetQueuedCompletionStatus(g_hiocp, &io_size, &key, (LPOVERLAPPED*)&over, INFINITE);
        int ci = static_cast<int>(key);

        if (!ret || io_size == 0) {
            DisconnectClient(ci);
            if (over && over->event_type == OP_SEND) delete over;
            continue;
        }

        if (over->event_type == OP_RECV) {
            unsigned char* buf = g_clients[ci].recv_over.IOCP_buf;
            unsigned psize = g_clients[ci].curr_packet_size;
            unsigned pr_size = g_clients[ci].prev_packet_data;
            while (io_size > 0) {
                if (psize == 0) psize = buf[0];
                if (io_size + pr_size >= psize) {
                    unsigned char packet[MAX_PACKET_SIZE];
                    memcpy(packet, g_clients[ci].packet_buf, pr_size);
                    memcpy(packet + pr_size, buf, psize - pr_size);
                    ProcessPacket(ci, packet);
                    io_size -= psize - pr_size;
                    buf += psize - pr_size;
                    psize = 0; pr_size = 0;
                }
                else {
                    memcpy(g_clients[ci].packet_buf + pr_size, buf, io_size);
                    pr_size += io_size;
                    io_size = 0;
                }
            }
            g_clients[ci].curr_packet_size = psize;
            g_clients[ci].prev_packet_data = pr_size;
            DWORD recv_flag = 0;
            int ret = WSARecv(g_clients[ci].client_socket, &g_clients[ci].recv_over.wsabuf, 1, NULL, &recv_flag, &g_clients[ci].recv_over.over, NULL);
            if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
                DisconnectClient(ci);
            }
        }
        else if (over->event_type == OP_SEND) {
            delete over;
        }
    }
}

void ClientThread(int ci) {
    while (g_clients[ci].connected) {
        this_thread::sleep_for(chrono::milliseconds(200));
        CS_TEST_MOVE_PACKET pkt;
        pkt.size = sizeof(pkt);
        pkt.type = CS_TEST_MOVE;
        pkt.pl.id = ci;
        pkt.pl.x = rand() % 100;
        pkt.pl.y = 0;
        pkt.pl.z = rand() % 100;
        pkt.pl.yaw = rand() % 360;
        pkt.pl.st = WALK;
        pkt.client_send_time = get_chrono_timestamp();

        SendPacket(ci, &pkt);
    }
}

void ConnectClient(int ci) {
    g_clients[ci].client_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

    SOCKADDR_IN addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUM);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret = WSAConnect(g_clients[ci].client_socket, (sockaddr*)&addr, sizeof(addr), NULL, NULL, NULL, NULL);
    if (ret != 0) {
        error_display("ConnectClient", WSAGetLastError());
        return;
    }

    g_clients[ci].connected = true;
    g_clients[ci].curr_packet_size = 0;
    g_clients[ci].prev_packet_data = 0;
    ZeroMemory(&g_clients[ci].recv_over, sizeof(g_clients[ci].recv_over));
    g_clients[ci].recv_over.event_type = OP_RECV;
    g_clients[ci].recv_over.wsabuf.buf = reinterpret_cast<CHAR*>(g_clients[ci].recv_over.IOCP_buf);
    g_clients[ci].recv_over.wsabuf.len = sizeof(g_clients[ci].recv_over.IOCP_buf);

    CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_clients[ci].client_socket), g_hiocp, ci, 0);
    DWORD recv_flag = 0;
    ret = WSARecv(g_clients[ci].client_socket, &g_clients[ci].recv_over.wsabuf, 1, NULL, &recv_flag, &g_clients[ci].recv_over.over, NULL);
    if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        error_display("WSARecv", WSAGetLastError());
        DisconnectClient(ci);
        return;
    }

    CS_LOGIN_PACKET login;
    login.size = sizeof(login);
    login.type = CS_LOGIN;
    sprintf_s(login.id, "test%04d", ci);
    sprintf_s(login.pw, "pw%04d", ci);
    SendPacket(ci, &login);

    CS_ENTER_GAME_PACKET enter{};
    enter.size = sizeof(enter);
    enter.type = CS_ENTER_GAME;
    swprintf_s(enter.name, L"Player%04d", ci);
    SendPacket(ci, &enter);

    thread(ClientThread, ci).detach();
}

void TryConnectLoop() {
    while (true) {
        if (num_connections >= MAX_TEST) break;

        int ci = num_connections;
        g_clients[ci].connected = false;

        ConnectClient(ci);

        if (g_clients[ci].connected) {
            ++num_connections;
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

int main() {
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);

    for (int i = 0; i < 4; ++i)
        thread(WorkerThread).detach();

    thread(TryConnectLoop).detach();

    while (true) {
        this_thread::sleep_for(chrono::seconds(5));
        double total = 0;
        int count = 0;
        for (int i = 0; i < num_connections; ++i) {
            lock_guard<mutex> lg(g_clients[i].ping_mutex);
            for (auto v : g_clients[i].ping_history) {
                total += v;
                count++;
            }
        }
        double avg = (count > 0) ? total / count : 0.0;
        cout << "[현재 클라 수: " << active_clients.load() << "] 평균 핑: " << avg << "ms\n";
    }

    WSACleanup();
    return 0;
}
