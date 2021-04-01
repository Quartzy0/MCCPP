//
// Created by quartzy on 3/28/21.
//

#ifndef MCCPP_PACKETPLUGINMESSAGE_H
#define MCCPP_PACKETPLUGINMESSAGE_H

#include "../Packet.h"
#include <vector>

namespace network{

    class PacketPluginMessage : public Packet{
    private:
        std::string channelName;
        std::vector<uint8_t> bytes;
    public:
        uint32_t getPacketId() override;

        uint32_t encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };
}

#endif //MCCPP_PACKETPLUGINMESSAGE_H
