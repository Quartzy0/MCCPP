//
// Created by quartzy on 3/29/21.
//

#ifndef MCCPP_PACKETHELDITEMCHANGE_H
#define MCCPP_PACKETHELDITEMCHANGE_H

#include "../../Packet.h"

namespace network{

    class PacketHeldItemChange : public Packet{
    private:
        uint8_t slot;
    public:
        uint32_t getPacketId() override;

        void encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //MCCPP_PACKETHELDITEMCHANGE_H
