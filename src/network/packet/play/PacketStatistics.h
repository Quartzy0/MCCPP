//
// Created by quartzy on 3/31/21.
//

#ifndef MCCPP_PACKETSTATISTICS_H
#define MCCPP_PACKETSTATISTICS_H

#include "../Packet.h"
#include <unordered_map>
#include <string>

namespace network{

    class PacketStatistics : public Packet{
    private:
        uint32_t count;
        std::unordered_map<std::string, int32_t> stats;
    public:
        uint32_t getPacketId() override;

        uint32_t encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //MCCPP_PACKETSTATISTICS_H
