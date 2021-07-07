//
// Created by quartzy on 3/28/21.
//

#include "PacketKeepAlive.h"
#include "../../../NetworkManager.h"

uint32_t network::PacketKeepAlive::getPacketId() {
    return 0x00;
}

void network::PacketKeepAlive::encode(uint8_t *out) {
    encodeVarInt(keepAliveId, out);
}

void network::PacketKeepAlive::decode(uint8_t *in, size_t size) {
    uint32_t decodedBytes;
    keepAliveId = decode_unsigned_varint(in, decodedBytes);
}

uint32_t network::PacketKeepAlive::size() {
    return varIntLen(keepAliveId);
}

void network::PacketKeepAlive::handlePacket() {
    PacketKeepAlive keepAlive{keepAliveId};
    network::NetworkManager::sendPacket(keepAlive);
}

network::PacketKeepAlive::PacketKeepAlive(uint32_t keepAliveId) : keepAliveId(keepAliveId) {}
