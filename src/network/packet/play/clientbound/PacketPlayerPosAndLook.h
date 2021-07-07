//
// Created by quartzy on 3/30/21.
//

#ifndef MCCPP_PACKETPLAYERPOSANDLOOK_H
#define MCCPP_PACKETPLAYERPOSANDLOOK_H

#include "../../Packet.h"

namespace network{

    class PacketPlayerPosAndLook : public Packet{
    private:
        double x, y, z;
        float yaw, pitch;
        uint8_t flags;
    public:
        uint32_t getPacketId() override;

        void encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //MCCPP_PACKETPLAYERPOSANDLOOK_H
