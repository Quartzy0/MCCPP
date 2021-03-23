//
// Created by quartzy on 3/20/21.
//

#ifndef MCCPP_BLOCK_H
#define MCCPP_BLOCK_H


#include <cstdint>
#include <unordered_map>
#include "../TextureAtlas.h"

class Block {
private:
    uint16_t textures[6]{};
    std::vector<uint8_t> animationFrames[6];
    uint8_t frameTime[6]{};
    uint16_t blockId;

    static std::unordered_map<uint16_t, Block> blocks;

    Block(TextureAtlas& atlas, uint16_t blockId, const char* blockName);
public:
    Block() = default;

    Block(uint16_t *textures, std::vector<std::vector<uint8_t>> &animationFrames, uint8_t *frameTime, uint16_t blockId);

    [[nodiscard]] uint16_t getBlockId() const;

    [[nodiscard]] const uint16_t* getTextures() const;

    static void initBlocks(TextureAtlas& textureAtlas);

    static Block* getBlockById(uint16_t id);
};


#endif //MCCPP_BLOCK_H
