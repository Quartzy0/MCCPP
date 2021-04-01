//
// Created by quartzy on 3/13/21.
//

#include "PacketLoginStart.h"

#include <utility>

uint32_t network::PacketLoginStart::getPacketId() {
    return 0;
}

uint32_t network::PacketLoginStart::encode(uint8_t *out) {
    return encodeString(username.c_str(), out);
}

uint32_t network::PacketLoginStart::size() {
    return strLen(username.c_str());
}

network::PacketLoginStart::PacketLoginStart(std::string& username) : username(std::move(username)) {}

void network::PacketLoginStart::decode(uint8_t *in, size_t size) {

}

void network::PacketLoginStart::handlePacket() {

}
