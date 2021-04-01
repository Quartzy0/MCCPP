//
// Created by quartzy on 3/28/21.
//

#ifndef MCCPP_PACKETSERVERDIFFICULTY_H
#define MCCPP_PACKETSERVERDIFFICULTY_H

#include "../Packet.h"

namespace network{

    class PacketServerDifficulty : public Packet{
    private:
        uint8_t difficulty;
    public:
        uint32_t getPacketId() override;

        uint32_t encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //MCCPP_PACKETSERVERDIFFICULTY_H
