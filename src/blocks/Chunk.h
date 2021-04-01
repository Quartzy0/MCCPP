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

class Position{
public:
    int32_t x, y ,z;

    Position() = default;
    ~Position() = default;
};

class Chunk {
private:
    Block* block[CX][CY][CZ];
    uint32_t lightLevel[CX][CY][CZ];
    uint32_t lightLevelsCorrected[CX][CY][CZ];
    GLuint vbo;
    GLuint textureID;
    uint32_t elements;
    bool changed;
    ShaderProgram& shaderProgram;
    int32_t posX,posY,posZ;

    Chunk* chunkPosX;
    Chunk* chunkNegX;
    Chunk* chunkPosY;
    Chunk* chunkNegY;
    Chunk* chunkPosZ;
    Chunk* chunkNegZ;

    void makeLightTexture();

    void updateLightTexture();
public:
    Chunk(ShaderProgram &shaderProgram1, Chunk *chunkPosX, Chunk *chunkNegX, Chunk *chunkPosY, Chunk *chunkNegY,
          Chunk *chunkPosZ, Chunk *chunkNegZ);

    Chunk() = default;

    ~Chunk();

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

    void setLightLevels(uint8_t* lightLevelsEncoded, uint8_t* lightLevelsEncodedBlock);
};


#endif //MCCPP_CHUNK_H
