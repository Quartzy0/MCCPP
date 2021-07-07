//
// Created by quartzy on 7/6/21.
//

#include "PacketPlayerPosition.h"

uint32_t network::PacketPlayerPosition::getPacketId() {
    return 0x04;
}

void network::PacketPlayerPosition::encode(uint8_t *out) {
    std::memcpy(out, &x, sizeof(double)*3 + sizeof(bool));
}

uint32_t network::PacketPlayerPosition::size() {
    return sizeof(double)*3 + sizeof(bool);
}

network::PacketPlayerPosition::PacketPlayerPosition(double x, double y, double z, bool onGround) : x(x), y(y), z(z),
                                                                                                   onGround(onGround) {}

void network::PacketPlayerPosition::decode(uint8_t *in, size_t size) {

}

void network::PacketPlayerPosition::handlePacket() {

}
