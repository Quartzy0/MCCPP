//
// Created by quartzy on 3/28/21.
//

#include <iostream>
#include "PacketPluginMessage.h"

uint32_t network::PacketPluginMessage::getPacketId() {
    return 0x3F;
}

void network::PacketPluginMessage::encode(uint8_t *out) {

}

void network::PacketPluginMessage::decode(uint8_t *in, size_t size) {
    int i = decodeString((const char*) in, channelName);
    bytes = std::vector<uint8_t>{};
    bytes.resize(size-i);
    std::memcpy(bytes.data(), in+i, size-i);
}

uint32_t network::PacketPluginMessage::size() {
    return 0;
}

void network::PacketPluginMessage::handlePacket() {
    if (channelName=="MC|Brand"){
        std::string brand{};
        decodeString((const char*) bytes.data(), brand);
        std::cout << "Server brand: " << brand << "\n";
    }
}
