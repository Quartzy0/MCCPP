//
// Created by quartzy on 3/28/21.
//

#ifndef MCCPP_PACKETKEEPALIVE_H
#define MCCPP_PACKETKEEPALIVE_H

#include "../Packet.h"

namespace network{

    class PacketKeepAlive : public Packet{
    private:
        uint32_t keepAliveId;
    public:
        PacketKeepAlive() = default;

        PacketKeepAlive(uint32_t keepAliveId);

        uint32_t getPacketId() override;

        uint32_t encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //MCCPP_PACKETKEEPALIVE_H
