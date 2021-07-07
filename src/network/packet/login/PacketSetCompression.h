//
// Created by quartzy on 3/13/21.
//

#ifndef TESTING1_PACKETSETCOMPRESSION_H
#define TESTING1_PACKETSETCOMPRESSION_H

#include <cstdint>
#include "../Packet.h"

namespace network{

    class PacketSetCompression : public Packet{
    private:
        int32_t threshold;
    public:
        uint32_t getPacketId() override;

        void decode(uint8_t *in, size_t size) override;

        void encode(uint8_t *out) override;

        uint32_t size() override;

        int32_t getThreshold() const;

        void handlePacket() override;
    };

}
#endif //TESTING1_PACKETSETCOMPRESSION_H
