//
// Created by quartzy on 3/13/21.
//

#include <iostream>
#include "PacketSetCompression.h"

#include "../../../MCCPP.h"
#include "../../NetworkManager.h"

uint32_t network::PacketSetCompression::getPacketId() {
    return 3;
}

void network::PacketSetCompression::decode(uint8_t *in, size_t size) {
    uint32_t thresholdBytes = 0;
    threshold = decode_signed_varint(in, thresholdBytes);
}

int32_t network::PacketSetCompression::getThreshold() const {
    return threshold;
}

void network::PacketSetCompression::encode(uint8_t *out) {

}

uint32_t network::PacketSetCompression::size() {
    return 0;
}

void network::PacketSetCompression::handlePacket() {
//    auto task = std::packaged_task<void()>{[&](){
//        std::cout << "Main thread GAMER " << std::to_string(threshold) << "\n";
//    }};
//    MCCPP::INSTANCE.addTask(std::forward<std::packaged_task<void()>>(task));
    NetworkManager::enableCompression(threshold);
    std::cout << "Compression threshold set as " << std::to_string(threshold) << "\n";
}
