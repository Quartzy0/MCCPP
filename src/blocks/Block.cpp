//
// Created by quartzy on 3/20/21.
//

#include <cstring>
#include "Block.h"

std::unordered_map<uint16_t, Block> Block::blocks;

const uint16_t* Block::getTextures() const {
    return &textures[0];
}

uint16_t Block::getBlockId() const {
    return blockId;
}

void Block::initBlocks(TextureAtlas& textureAtlas) {
    blocks[1] = Block(textureAtlas, 1, "stone.png");
    blocks[3] = Block(textureAtlas, 3, "dirt.png");
    blocks[56] = Block(textureAtlas, 56, "diamond_ore.png");
    blocks[87] = Block(textureAtlas, 87, "netherrack.png");
    blocks[4] = Block(textureAtlas, 4, "cobblestone.png");
}

Block *Block::getBlockById(uint16_t id) {
    return &blocks[id];
}

Block::Block(uint16_t *texturesIn, std::vector<std::vector<uint8_t>> &animationFramesIn, uint8_t *frameTimeIn, uint16_t blockId) :
                                                                                                   blockId(blockId) {
    std::memcpy(textures, texturesIn, 6 * sizeof(uint16_t));
    std::memcpy(frameTime, frameTimeIn, 6 * sizeof(uint8_t));

    for (int i = 0; i < 6; ++i) {
        animationFrames[i] = std::vector<uint8_t>{};
        animationFrames[i].resize(animationFramesIn.size());
        std::memcpy(animationFrames[i].data(), animationFramesIn[i].data(), animationFramesIn[i].size());
    }
}

Block::Block(TextureAtlas& atlas, uint16_t blockId, const char* blockName) : blockId(blockId) {
    uint16_t texId = atlas.getTextureFromName(blockName);
    for (int i = 0; i < 6; ++i) {
        textures[i] = texId;
    }

    if (atlas.isAnimated(texId)){
        for (int i = 0; i < 6; ++i) {
            animationFrames[i] = std::vector<uint8_t>{};
            std::vector<uint8_t> original = atlas.getFrames(texId);
            animationFrames[i].resize(original.size());
            for (int j = 0; j < original.size(); ++j) {
                animationFrames[i][j] = original[j];
            }
            frameTime[i] = atlas.getFrameTime(texId);
        }
    }
}
