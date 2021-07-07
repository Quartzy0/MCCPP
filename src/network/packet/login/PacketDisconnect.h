//
// Created by quartzy on 3/14/21.
//

#ifndef TESTING1_PACKETDISCONNECT_H
#define TESTING1_PACKETDISCONNECT_H

#include "../Packet.h"

namespace network{

class PacketDisconnect : public Packet{
private:
    std::string reason;
public:
    uint32_t getPacketId() override;

    void encode(uint8_t *out) override;

    void decode(uint8_t *in, size_t size) override;

    uint32_t size() override;

    void handlePacket() override;
};

}
#endif //TESTING1_PACKETDISCONNECT_H
