#include "stdafx.h"

std::atomic_int id = 0;
std::array<Player, MAX_PLAYER> players{};
std::array<Kid, MAX_NPC> npcs{};
std::mutex players_mutex;
std::array<Door, MAX_DOOR> doors{};
concurrency::concurrent_unordered_map<std::string, Room*> rooms{};
float f_time;
std::chrono::high_resolution_clock::time_point startTime;

unsigned short g_player_count = 0;
unsigned short npc_count = 0; 

int setid()
{
    g_player_count++;
    return id++;
}

void server_error(const char* msg)
{
    printf("%s with error: %d\n", msg, WSAGetLastError());
    exit(1);
}
