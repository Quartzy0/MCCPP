//
// Created by quartzy on 3/14/21.
//

#include <iostream>
#include "PacketDisconnect.h"

uint32_t network::PacketDisconnect::getPacketId() {
    return 0;
}

void network::PacketDisconnect::encode(uint8_t *out) {

}

void network::PacketDisconnect::decode(uint8_t *in, size_t size) {
    decodeString((const char*)in, reason);
}

uint32_t network::PacketDisconnect::size() {
    return 0;
}

void network::PacketDisconnect::handlePacket() {
    std::cout << "Disconnected: " << reason << "\n";
}
