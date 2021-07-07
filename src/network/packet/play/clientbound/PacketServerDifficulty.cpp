//
// Created by quartzy on 3/28/21.
//

#include <iostream>
#include "PacketServerDifficulty.h"

uint32_t network::PacketServerDifficulty::getPacketId() {
    return 0x41;
}

void network::PacketServerDifficulty::encode(uint8_t *out) {

}

void network::PacketServerDifficulty::decode(uint8_t *in, size_t size) {
    difficulty = in[0];
}

uint32_t network::PacketServerDifficulty::size() {
    return 0;
}

void network::PacketServerDifficulty::handlePacket() {
    std::cout << "Difficulty changed to " << std::to_string(difficulty) << "\n";
}
