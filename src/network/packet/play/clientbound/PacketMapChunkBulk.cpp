//
// Created by quartzy on 3/31/21.
//

#include "PacketMapChunkBulk.h"
#include "../../../../util/Log.h"
#include "../../../../MCCPP.h"

bool network::PacketMapChunkBulk::first;

uint32_t network::PacketMapChunkBulk::getPacketId() {
    return 0x26;
}

void network::PacketMapChunkBulk::encode(uint8_t *out) {

}

void network::PacketMapChunkBulk::decode(uint8_t *in, size_t size) {
    uint32_t i = 0;
    skylightSent = in[i++];
    uint32_t chunkColumnCountLen = 0;
    chunkColumnCount = decode_unsigned_varint(in+i, chunkColumnCountLen);
    i+=chunkColumnCountLen;


    chunkData = std::vector<ChunkColumn>{};
    chunkData.reserve(chunkColumnCount);

    for (uint32_t j = 0; j < chunkColumnCount; ++j) {
        ChunkColumn chunkColumn{};
        i+=decodeInt(in+i, chunkColumn.chunkX);
        i+=decodeInt(in+i, chunkColumn.chunkZ);
        i+=decodeShort(in+i, chunkColumn.bitMask);

        chunkData.push_back(chunkColumn);
    }

    for (uint32_t j = 0; j < chunkColumnCount; ++j) {
        ChunkColumn& column = chunkData[j];
        for (int k = 0; k < 16; ++k) {
            if ((column.bitMask & (1 << k)) != 0){
                std::memcpy(column.chunks[k].blockIds, in+i, 4096 * sizeof(uint16_t));
                i+=4096 * sizeof(uint16_t);
            }
        }
        for (int k = 0; k < 16; ++k) {
            if ((column.bitMask & (1 << k)) != 0){
                std::memcpy(column.chunks[k].lightLevels, in+i, 2048);
                i+=2048;
            }
        }
        if (skylightSent){
            for (int k = 0; k < 16; ++k) {
                if ((column.bitMask & (1 << k)) != 0){
                    std::memcpy(column.chunks[k].skylightLevels, in+i, 2048);
                    i+=2048;
                }
            }
        }
        std::memcpy(column.biomeIDs, in+i, 256);
        i+=256;
    }
}

uint32_t network::PacketMapChunkBulk::size() {
    return 0;
}

void network::PacketMapChunkBulk::handlePacket() {
    std::vector<ChunkColumn> chunks = chunkData;
    auto task = std::packaged_task<void()>{[chunks](){
        Block* dirt = Block::getBlockById(3);
        Block* stone = Block::getBlockById(1);
//        static bool f = true;
        for (int i = 0; i < chunks.size(); ++i) {
            ChunkColumn column = chunks[i];
            for (int k = 0; k < 16; ++k) {
                if (column.bitMask & (1 << k)){
                    Chunk* chunk = MCCPP::superchunk->getOrMakeChunk(column.chunkX, k, column.chunkZ);
                    if (!chunk) continue;
//                    if (f){
//                        MCCPP::playerController->setPos(glm::vec<3, double, glm::defaultp>{column.chunkX*16, k*16, column.chunkZ*16});
//                        f = false;
//                    }
                    for (int y = 0; y < 16; ++y) {
                        for (int x = 0; x < 16; ++x) {
                            for (int z = 0; z < 16; ++z) {
                                /*
                                 *  ID              Meta
                                 *  V                V
                                 * 100110010010     1101
                                 */
                                if ((column.chunks[k].blockIds[y << 8 | z << 4 | x] >> 4)==50){
                                    chunk->set(x, y, z, dirt);
                                }else if((column.chunks[k].blockIds[y << 8 | z << 4 | x] >> 4)!=0){
                                    chunk->set(x, y, z, stone);
                                }
                            }
                        }
                    }
                    MCCPP::superchunk->updateNearbyChunks(column.chunkX, k, column.chunkZ);
                }
            }
        }
    }};
    MCCPP::addTask(std::forward<std::packaged_task<void()>>(task));
}
