//
// Created by quartzy on 2/16/21.
//

#ifndef MCCPP_CHUNK_H
#define MCCPP_CHUNK_H

#include "../graphics/ShaderProgram.h"
#include "Block.h"
#include <cstdint>
#include <GL/glew.h>

#define CX 16
#define CY 16
#define CZ 16


class Chunk {
private:
    Block* block[CX][CY][CZ];
    GLuint vbo;
    uint32_t elements;
    bool changed;
    ShaderProgram& shaderProgram;

    Chunk* chunkPosX;
    Chunk* chunkNegX;
    Chunk* chunkPosY;
    Chunk* chunkNegY;
    Chunk* chunkPosZ;
    Chunk* chunkNegZ;
public:
    Chunk(ShaderProgram &shaderProgram1, Chunk *chunkPosX, Chunk *chunkNegX, Chunk *chunkPosY, Chunk *chunkNegY,
          Chunk *chunkPosZ, Chunk *chunkNegZ);

    ~Chunk();

    Block* get(uint32_t x, uint32_t y, uint32_t z);

    void set(uint32_t x, uint32_t y, uint32_t z, Block* type);

    void update();

    void render();

    void cleanup();

    void makeNeedUpdate(Chunk* chunkPosX,Chunk* chunkNegX,Chunk* chunkPosY,Chunk* chunkNegY,Chunk* chunkPosZ,Chunk* chunkNegZ);
};


#endif //MCCPP_CHUNK_H
