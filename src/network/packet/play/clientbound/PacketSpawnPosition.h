//
// Created by quartzy on 3/28/21.
//

#ifndef MCCPP_PACKETSPAWNPOSITION_H
#define MCCPP_PACKETSPAWNPOSITION_H

#include "../../Packet.h"

namespace network{

    class PacketSpawnPosition : public Packet{
    private:
        int32_t x,y,z;
    public:
        uint32_t getPacketId() override;

        void encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //MCCPP_PACKETSPAWNPOSITION_H
