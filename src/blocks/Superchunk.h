//
// Created by quartzy on 2/17/21.
//

#ifndef MCCPP_SUPERCHUNK_H
#define MCCPP_SUPERCHUNK_H

#define SCX 8
#define SCY 8
#define SCZ 8

#define CHUNK_CULLING

#include "Chunk.h"

class Superchunk {
private:
    Chunk* chunk[SCX][SCY][SCZ];
    ShaderProgram& shaderProgram;
public:

    Superchunk(ShaderProgram& shaderProgram);

    ~Superchunk();

    uint16_t get(int32_t x, int32_t y, int32_t z);

    void set(int32_t x, int32_t y, int32_t z, uint16_t type);

    void render(glm::mat4 vp);
};


#endif //MCCPP_SUPERCHUNK_H
