//
// Created by quartzy on 3/14/21.
//

#ifndef TESTING1_PACKETJOINGAME_H
#define TESTING1_PACKETJOINGAME_H

#include "../Packet.h"

namespace network{

class PacketJoinGame : public Packet{
private:
    int32_t entityId;
    uint8_t gamemode;
    int8_t dim;
    uint8_t difficulty;
    uint8_t maxPlayerList;
    std::string levelType;
    bool reducedDebugInfo;
public:
    uint32_t getPacketId() override;

    uint32_t encode(uint8_t *out) override;

    void decode(uint8_t *in, size_t size) override;

    uint32_t size() override;

    void handlePacket() override;
};

}

#endif //TESTING1_PACKETJOINGAME_H
