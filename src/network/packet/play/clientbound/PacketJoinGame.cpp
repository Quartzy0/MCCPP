//
// Created by quartzy on 3/14/21.
//

#include "PacketJoinGame.h"

uint32_t network::PacketJoinGame::getPacketId() {
    return 1;
}

void network::PacketJoinGame::encode(uint8_t *out) {

}

void network::PacketJoinGame::decode(uint8_t *in, size_t size) {
    int i = decodeInt(in, entityId);
    gamemode = in[i++];
    dim = in[i++];
    difficulty = in[i++];
    maxPlayerList = in[i++];
    i+=decodeString((const char*) in+i, levelType);
    reducedDebugInfo = in[i++];
}

uint32_t network::PacketJoinGame::size() {
    return 0;
}

void network::PacketJoinGame::handlePacket() {
    printf("Dim: %d, EntityId: %d, Difficulty: %u, Level type: %s, Gamemode: %u\n", dim, entityId, difficulty, levelType.c_str(), gamemode);
}
