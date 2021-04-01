//
// Created by quartzy on 3/30/21.
//

#ifndef MCCPP_PACKETPLAYERABILITIES_H
#define MCCPP_PACKETPLAYERABILITIES_H

#include "../Packet.h"

namespace network{

    class PacketPlayerAbilities : public Packet{
    private:
        uint8_t flags;
        float flySpeed;
        float walkSpeed;
    public:
        uint32_t getPacketId() override;

        uint32_t encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}


#endif //MCCPP_PACKETPLAYERABILITIES_H
