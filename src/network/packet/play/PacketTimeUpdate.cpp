//
// Created by quartzy on 3/31/21.
//

#include "PacketTimeUpdate.h"
#include "../../../MCCPP.h"

uint32_t network::PacketTimeUpdate::getPacketId() {
    return 0x03;
}

uint32_t network::PacketTimeUpdate::encode(uint8_t *out) {
    return 0;
}

void network::PacketTimeUpdate::decode(uint8_t *in, size_t size) {
    uint32_t i = decodeLong(in, worldTime);
    i+=decodeLong(in+i, timeOfDay);
}

uint32_t network::PacketTimeUpdate::size() {
    return 0;
}

void network::PacketTimeUpdate::handlePacket() {
    int64_t cpyTime = timeOfDay, cpyWorld = worldTime;
    auto task = std::packaged_task<void()>{[=](){
        MCCPP::timeOfDay = cpyTime;
        MCCPP::worldAge = cpyWorld;
    }};
    MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
}
