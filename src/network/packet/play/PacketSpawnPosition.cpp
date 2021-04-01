//
// Created by quartzy on 3/28/21.
//

#include <iostream>
#include "PacketSpawnPosition.h"
#include "../../../MCCPP.h"
#include "../../../util/Log.h"

uint32_t network::PacketSpawnPosition::getPacketId() {
    return 0;
}

uint32_t network::PacketSpawnPosition::encode(uint8_t *out) {
    return 0;
}

void network::PacketSpawnPosition::decode(uint8_t *in, size_t size) {
    decodeBlockPos(in, x, y, z);
}

uint32_t network::PacketSpawnPosition::size() {
    return 0;
}

void network::PacketSpawnPosition::handlePacket() {
    int32_t x1 = x, y1 = y, z1 = z;
    auto task = std::packaged_task<void()>([=](){
        CORE_INFO("Spawned at x: {} y:{} z:{}", x1, y1 ,z1);
        MCCPP::playerController->setX(x1);
        MCCPP::playerController->setY(y1);
        MCCPP::playerController->setZ(z1);
    });
    MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
}
