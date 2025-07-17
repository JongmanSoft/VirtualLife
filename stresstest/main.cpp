
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <memory>
#include <atomic>
#include "../server/stdafx.h"

#pragma comment(lib, "ws2_32.lib")

const char* SERVER_IP = "127.0.0.1";

struct LoadClient {
    SOCKET sock;
    std::atomic<bool> running{ true };
    std::thread th;
    std::mutex ping_mutex;
    std::vector<double> ping_history;

    bool Connect() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) return false;

        SOCKADDR_IN server;
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT_NUM);
        server.sin_addr.s_addr = inet_addr(SERVER_IP);

        if (connect(sock, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR) {
            closesocket(sock);
            return false;
        }
        return true;
    }

    void Start() {
        th = std::thread(&LoadClient::run, this);
    }

    void Stop() {
        running = false;
        closesocket(sock);
        if (th.joinable()) th.join();
    }

    void run() {
        while (running) {
            auto start = std::chrono::high_resolution_clock::now();

            CS_MOVE_PACKET p;
            PlayerInfo pi;
			pi.gold = 0;


            p.size = sizeof(CS_MOVE_PACKET);
            p.pl = PlayerInfo{};
            p.type = CS_MOVEP;

            send(sock, (char*)&p, sizeof(p), 0);

            char buf[BUFSIZE];
            recv(sock, buf, BUFSIZE, 0); // 간단히 수신 처리

            auto end = std::chrono::high_resolution_clock::now();
            double ping_ms = std::chrono::duration<double, std::milli>(end - start).count();

            {
                std::lock_guard<std::mutex> lg(ping_mutex);
                ping_history.push_back(ping_ms);
                if (ping_history.size() > 100) ping_history.erase(ping_history.begin());
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    double GetAvgPing() {
        std::lock_guard<std::mutex> lg(ping_mutex);
        if (ping_history.empty()) return 0.0;
        double sum = 0;
        for (auto v : ping_history) sum += v;
        return sum / ping_history.size();
    }
};

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    std::vector<std::shared_ptr<LoadClient>> clients;

    while (true) {
        // 평균 핑 계산
        double totalPing = 0;
        int count = 0;
        for (auto& c : clients) {
            totalPing += c->GetAvgPing();
            ++count;
        }
        double avgPing = count > 0 ? totalPing / count : 0.0;

        std::cout << "[현재 클라 수: " << clients.size() << "] 평균 핑: " << avgPing << "ms\n";

        if (avgPing < 150 && clients.size() < 1000) {
            // 클라 추가
            for (int i = 0; i < 10; ++i) {
                auto client = std::make_shared<LoadClient>();
                if (client->Connect()) {
                    client->Start();
                    clients.push_back(client);
                }
            }
        }
        else if (avgPing > 200 && clients.size() > 10) {
            // 클라 감소
            for (int i = 0; i < 10 && !clients.empty(); ++i) {
                clients.back()->Stop();
                clients.pop_back();
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    for (auto& c : clients) c->Stop();
    WSACleanup();
    return 0;
}
