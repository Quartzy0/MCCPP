//
// Created by quartzy on 3/28/21.
//

#include "MCCPP.h"

std::queue<std::packaged_task<void()>> MCCPP::queue;

std::vector<PlayerMP> MCCPP::players;

std::mutex MCCPP::taskQueueMutex;
std::string MCCPP::username;
std::string MCCPP::uuid;
std::string MCCPP::authToken;
std::string MCCPP::serverIP;
uint16_t MCCPP::port;

PlayerController* MCCPP::playerController;

std::unordered_map<std::string, int32_t> MCCPP::statistics;
int64_t MCCPP::worldAge, MCCPP::timeOfDay;
Superchunk* MCCPP::superchunk;

void MCCPP::tick(float deltaTime) {
    if (playerController) playerController->tick(deltaTime);

    while(!queue.empty()){
        if (taskQueueMutex.try_lock()){
            std::packaged_task<void()>& task = queue.front();
            task();
            queue.pop();
            taskQueueMutex.unlock();
        }
    }
}

void MCCPP::addTask(std::packaged_task<void()> &&task) {
    taskQueueMutex.lock();
    queue.push(std::forward<std::packaged_task<void()>>(task));
    taskQueueMutex.unlock();
}
