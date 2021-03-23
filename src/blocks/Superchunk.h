//
// Created by quartzy on 2/17/21.
//

#ifndef MCCPP_SUPERCHUNK_H
#define MCCPP_SUPERCHUNK_H

#define SCX 8
#define SCY 8
#define SCZ 8

//#define CHUNK_CULLING

#include "Chunk.h"

class Superchunk {
private:
    Chunk* chunk[SCX][SCY][SCZ];
    ShaderProgram& shaderProgram;
    uint8_t tickCount = 0;
    double lastTickTime = -1;

    void updateChunk(uint32_t x, uint32_t y, uint32_t z);
public:

    Superchunk(ShaderProgram& shaderProgram);

    ~Superchunk();

    Block* get(int32_t x, int32_t y, int32_t z);

    void set(int32_t x, int32_t y, int32_t z, Block* type);

    void render(glm::mat4 vp);

    Chunk* getChunkAt(uint32_t x, uint32_t y, uint32_t z);
};


#endif //MCCPP_SUPERCHUNK_H
