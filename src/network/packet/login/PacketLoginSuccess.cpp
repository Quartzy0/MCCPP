//
// Created by quartzy on 3/14/21.
//

#include <iostream>
#include "PacketLoginSuccess.h"
#include "../../NetworkManager.h"

uint32_t network::PacketLoginSuccess::getPacketId() {
    return 2;
}

uint32_t network::PacketLoginSuccess::encode(uint8_t *out) {
    return 0;
}

void network::PacketLoginSuccess::decode(uint8_t *in, size_t size) {
    int i = decodeString((const char*) in, uuid);
    i+=decodeString((const char*)in+i, username);
}

uint32_t network::PacketLoginSuccess::size() {
    return 0;
}

void network::PacketLoginSuccess::handlePacket() {
    std::cout << "Successfully logged in as " << username << " (" << uuid << ")\n";
    NetworkManager::setState(State::PLAY);
}
