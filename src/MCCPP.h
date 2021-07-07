//
// Created by quartzy on 2/7/21.
//
#ifndef MCCPP_MCCPP_H
#define MCCPP_MCCPP_H

#define USE_NETWORK

#include <future>
#include <queue>
#include "PlayerController.h"
#include "PlayerMP.h"


namespace MCCPP{
    extern std::queue<std::packaged_task<void()>> queue;
    extern std::mutex taskQueueMutex;

    extern std::string username;
    extern std::string uuid;
    extern std::string authToken;
    extern std::string serverIP;
    extern uint16_t port;
    extern std::vector<PlayerMP> players;

    extern PlayerController* playerController;
    extern Superchunk* superchunk;


    extern std::unordered_map<std::string, int32_t> statistics;
    extern int64_t worldAge, timeOfDay;

    void tick(float deltaTime);

    void addTask(std::packaged_task<void()>&& task);
};

#endif //MCCPP_MCCPP_H
