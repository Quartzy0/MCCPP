//
// Created by quartzy on 3/13/21.
//

#include "PacketHandshake.h"

#include <utility>

namespace network{

    uint32_t PacketHandshake::getPacketId() {
        return 0;
    }

    void PacketHandshake::encode(uint8_t *out) {
        int i = 0;
        i+=encodeVarInt(protocolVersion, out);
        i+=encodeString(serverAddress.data(), out+i);
        i+=encodeShort(port, out+i);
        i+=encodeVarInt(nextState, out+i);
    }

    PacketHandshake::PacketHandshake(uint32_t protocolVersion, std::string& serverAddress, uint16_t port,
                                     uint32_t nextState) : protocolVersion(protocolVersion), serverAddress(std::move(serverAddress)),
                                                           port(port), nextState(nextState), Packet() {}

    uint32_t PacketHandshake::size() {
        return strLen(serverAddress.c_str()) + varIntLen(protocolVersion) + 2 + varIntLen(nextState);
    }

    void PacketHandshake::decode(uint8_t *in, size_t size) {

    }

    void PacketHandshake::handlePacket() {

    }
}