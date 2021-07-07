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
    friend class Superchunk;
private:
    Block* block[CX][CY][CZ];
    GLuint vbo;
    GLuint vao;
    GLuint bufferId;
    uint32_t elements;
    bool changed;
    ShaderProgram* shaderProgram;
    int32_t posX,posY,posZ;
    int32_t arrayPosX, arrayPosY, arrayPosZ;

    Chunk* chunkPosX;
    Chunk* chunkNegX;
    Chunk* chunkPosY;
    Chunk* chunkNegY;
    Chunk* chunkPosZ;
    Chunk* chunkNegZ;
public:
    static void init();

    Chunk(ShaderProgram *shaderProgram1, Chunk *chunkPosX, Chunk *chunkNegX, Chunk *chunkPosY, Chunk *chunkNegY,
          Chunk *chunkPosZ, Chunk *chunkNegZ, int32_t arrayPosX, int32_t arrayPosY, int32_t arrayPosZ);

    Chunk() = default;

    ~Chunk();

    Chunk(const Chunk& other) = default;
    Chunk(Chunk&& other) = delete;
    Chunk& operator=(const Chunk& other) = delete;
    Chunk& operator=(Chunk&& other) = delete;

    Block* get(uint32_t x, uint32_t y, uint32_t z);

    void set(uint32_t x, uint32_t y, uint32_t z, Block* type);

    void update();

    void render();


    void cleanup();

    void makeNeedUpdate(Chunk* chunkPosX,Chunk* chunkNegX,Chunk* chunkPosY,Chunk* chunkNegY,Chunk* chunkPosZ,Chunk* chunkNegZ);

    int32_t getPosX() const;

    void setPosX(int32_t posX);

    int32_t getPosY() const;

    void setPosY(int32_t posY);

    int32_t getPosZ() const;

    void setPosZ(int32_t posZ);

    int32_t getArrayPosX() const;

    int32_t getArrayPosY() const;

    int32_t getArrayPosZ() const;
};


#endif //MCCPP_CHUNK_H
