//
// Created by quartzy on 3/31/21.
//

#ifndef MCCPP_PACKETMAPCHUNKBULK_H
#define MCCPP_PACKETMAPCHUNKBULK_H

#include <vector>
#include "../../Packet.h"

namespace network{

    class PacketMapChunkBulk : public Packet{
    private:
        static bool first;

        struct ChunkData{
            //16*16*16 array of block ids (2 bytes per block)
            uint16_t blockIds[4096];
            //16*16*16 array of nibbles (4 bits)
            uint8_t lightLevels[2048];
            //16*16*16 array of nibbles (4 bits)
            uint8_t skylightLevels[2048];
        };

        struct ChunkColumn{
            int32_t chunkX;
            int32_t chunkZ;
            uint16_t bitMask;

            //16*16 array of biome ids (1 byte for each y column)
            uint8_t biomeIDs[256];

            ChunkData chunks[16];
        };

        bool skylightSent;
        uint32_t chunkColumnCount;

        std::vector<ChunkColumn> chunkData;
    public:
        uint32_t getPacketId() override;

        void encode(uint8_t *out) override;

        void decode(uint8_t *in, size_t size) override;

        uint32_t size() override;

        void handlePacket() override;
    };

}

#endif //MCCPP_PACKETMAPCHUNKBULK_H
