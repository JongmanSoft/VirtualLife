#pragma once

class RoomManager {
public:
    static concurrency::concurrent_unordered_map<std::string, Room*>& Get() {
        static concurrency::concurrent_unordered_map<std::string, Room*> instance;
        return instance;
    }
};


