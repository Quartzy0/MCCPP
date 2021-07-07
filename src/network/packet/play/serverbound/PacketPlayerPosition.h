//
// Created by quartzy on 7/6/21.
//

#ifndef MCCPP_PACKETPLAYERPOSITION_H
#define MCCPP_PACKETPLAYERPOSITION_H

#include "../../Packet.h"
namespace network{

class PacketPlayerPosition : public Packet{
private:
    double x, y, z;
    bool onGround;
public:
    PacketPlayerPosition(double x, double y, double z, bool onGround);

    uint32_t getPacketId() override;

    void encode(uint8_t *out) override;

    uint32_t size() override;

    void decode(uint8_t *in, size_t size) override;

    void handlePacket() override;
};

}

#endif //MCCPP_PACKETPLAYERPOSITION_H
