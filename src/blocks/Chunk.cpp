//
// Created by quartzy on 2/16/21.
//

#include <cstring>
#include <glm/common.hpp>
#include "Chunk.h"

#include "../graphics/Vertex.h"
#include "../util/Log.h"

#include <algorithm>

Chunk::Chunk(ShaderProgram &shaderProgram1, Chunk *chunkPosX, Chunk *chunkNegX, Chunk *chunkPosY, Chunk *chunkNegY,
             Chunk *chunkPosZ, Chunk *chunkNegZ)
        : shaderProgram(shaderProgram1), chunkPosX(chunkPosX), chunkNegX(chunkNegX), chunkPosY(chunkPosY),
          chunkNegY(chunkNegY), chunkPosZ(chunkPosZ), chunkNegZ(chunkNegZ) {
    std::memset(block, 0, sizeof(block));
    elements = 0;
    changed = true;
    glGenBuffers(1, &vbo);

    makeLightTexture();
}

Chunk::~Chunk() {
    cleanup();
}

Block* Chunk::get(uint32_t x, uint32_t y, uint32_t z) {
    return block[x][y][z];
}

void Chunk::set(uint32_t x, uint32_t y, uint32_t z, Block* type) {
    block[x][y][z] = type;
    changed = true;
}

void Chunk::update() {
    changed = false;

    Vertex vertex[CX * CY * CZ * 6 * 6];
    int i = 0;

    //Negative x
    for(int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            bool visible = false;
            for (int z = 0; z < CZ; z++) {
                Block* type = block[x][y][z];

                if (!type || ((chunkNegX && x==0 && chunkNegX->get(CX-1, y, z)) || (x!=0 && block[x-1][y][z]))){
                    visible = false;
                    continue;
                }

                // View from negative x
                if(visible && type == block[x][y][z - 1]) {
                    vertex[i-1].z = z + 1;
                }else {
                    vertex[i++] = Vertex(x, y, z, type->getTextures()[0]);
                    vertex[i++] = Vertex(x, y+1, z+1, type->getTextures()[0]);
                    visible = true;
                }
            }
        }
    }

    //Positive x
    for(int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            bool visible = false;
            for (int z = 0; z < CZ; z++) {
                Block* type = block[x][y][z];

                if (!type || ((chunkPosX && x==CX-1 && chunkPosX->get(0, y, z)) || (x!=CX-1 && block[x+1][y][z]))){
                    visible = false;
                    continue;
                }

                // View from positive x
                if(visible && type == block[x][y][z - 1]) {
                    vertex[i-1].z = z + 1;
                }else{
                    vertex[i++] = Vertex(x+1, y, z, type->getTextures()[1]);
                    vertex[i++] = Vertex(x+1, y+1, z+1, type->getTextures()[1]);
                    visible = true;
                }
            }
        }
    }

    //Negative y
    for(int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            bool visible = false;
            for (int z = 0; z < CZ; z++) {
                Block* type = block[x][y][z];

                if (!type || ((chunkNegY && y==0 && chunkNegY->get(x, CY-1, z)) || (y!=0 && block[x][y-1][z]))){
                    visible = false;
                    continue;
                }

                // View from negative y
                if(visible && type == block[x][y][z - 1]) {
                    vertex[i-1].z = z + 1;
                }else{
                    vertex[i++] = Vertex(x, y, z, -type->getTextures()[2]);
                    vertex[i++] = Vertex(x+1, y, z+1, -type->getTextures()[2]);
                    visible = true;
                }
            }
        }
    }

    //Positive y
    for(int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            bool visible = false;
            for (int z = 0; z < CZ; z++) {
                Block* type = block[x][y][z];

                if (!type || ((chunkPosY && y==CY-1 && chunkPosY->get(x, 0, z)) || (y!=CY-1 && block[x][y+1][z]))){
                    visible = false;
                    continue;
                }

                // View from positive y
                if(visible && type == block[x][y][z - 1]) {
                    vertex[i-1].z = z + 1;
                }else {
                    vertex[i++] = Vertex(x, y+1, z, -type->getTextures()[3]);
                    vertex[i++] = Vertex(x+1, y+1, z+1, -type->getTextures()[3]);
                    visible = true;
                }
            }
        }
    }

    //Negative z
    for(int x = 0; x < CX; x++) {
        for (int z = 0; z < CZ; z++) {
            bool visible = false;
            for (int y = 0; y < CY; y++) {
                Block* type = block[x][y][z];

                if (!type || ((chunkNegZ && z==0 && chunkNegZ->get(x, y, CZ-1)) || (z!=0 && block[x][y][z-1]))){
                    visible = false;
                    continue;
                }

                // View from negative z
                if(visible && type == block[x][y - 1][z]) {
                    vertex[i-1].y = y + 1;
                }else {
                    vertex[i++] = Vertex(x, y, z, type->getTextures()[4]);
                    vertex[i++] = Vertex(x+1, y+1, z, type->getTextures()[4]);
                    visible = true;
                }
            }
        }
    }

    //Positive z
    for(int x = 0; x < CX; x++) {
        for (int z = 0; z < CZ; z++) {
            bool visible = false;
            for (int y = 0; y < CY; y++) {
                Block* type = block[x][y][z];

                if (!type || ((chunkPosZ && z==CZ-1 && chunkPosZ->get(x, y, 0)) || (z!=CZ-1 && block[x][y][z+1]))){
                    visible = false;
                    continue;
                }

                // View from positive z
                if(visible && type == block[x][y - 1][z]) {
                    vertex[i-1].y = y + 1;
                }else {
                    vertex[i++] = Vertex(x, y, z+1, type->getTextures()[5]);
                    vertex[i++] = Vertex(x+1, y+1, z+1, type->getTextures()[5]);
                    visible = true;
                }
            }
        }
    }

    elements = i;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, elements * sizeof(Vertex), vertex, GL_STATIC_DRAW);
}

void Chunk::render() {
    if (changed)
        update();
    if (!elements){
        return;
    }

//    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

//    glEnable(GL_CULL_FACE);
//    glFrontFace(GL_CCW);
//    glCullFace(GL_BACK);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    shaderProgram.specifyVertexAttributes();

//    shaderProgram.setUniformLightLevels(&lightLevel[0][0][0], CX*CY*CZ);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, textureID);
    shaderProgram.setUniform("lightLevel", 1);

    glDrawArrays(GL_LINES, 0, elements);
}

void Chunk::cleanup() {
    glDeleteBuffers(1, &vbo);

    glBindTexture(GL_TEXTURE_3D, 0);
    glDeleteTextures(1, &textureID);
}

void Chunk::makeNeedUpdate(Chunk* chunkPosX,Chunk* chunkNegX,Chunk* chunkPosY,Chunk* chunkNegY,Chunk* chunkPosZ,Chunk* chunkNegZ) {
    changed = true;
    this->chunkPosX = chunkPosX;
    this->chunkNegX = chunkNegX;
    this->chunkPosY = chunkPosY;
    this->chunkNegY = chunkNegY;
    this->chunkPosZ = chunkPosZ;
    this->chunkNegZ = chunkNegZ;
}

int32_t Chunk::getPosX() const {
    return posX;
}

void Chunk::setPosX(int32_t posX) {
    Chunk::posX = posX;
}

int32_t Chunk::getPosY() const {
    return posY;
}

void Chunk::setPosY(int32_t posY) {
    Chunk::posY = posY;
}

int32_t Chunk::getPosZ() const {
    return posZ;
}

void Chunk::setPosZ(int32_t posZ) {
    Chunk::posZ = posZ;
}

void Chunk::setLightLevels(uint8_t *lightLevelsEncoded, uint8_t* lightLevelsEncodedBlock) {
    struct LightPair{
        uint8_t l1:4;
        uint8_t l2:4;
    };
    for (int y = 0; y < CY; ++y) {
        for (int x = 0; x < CX; ++x) {
            for (int z = 0; z < CZ; ++z) {
                LightPair* lightPair = (LightPair*) lightLevelsEncodedBlock + (y << 8 | z << 4 | x);
                lightLevel[x][y][z++] = lightPair->l1 << 4;
                lightLevel[x][y][z] = lightPair->l2 << 4;
            }
        }
    }
    for (int x = 0; x < CX; ++x) {
        for (int y = 0; y < CY; ++y) {
            for (int z = 0; z < CZ; ++z) {
                uint32_t lightLevelUp = lightLevel[x][y+1][z];
                uint32_t lightLevelNorth = lightLevel[x][y][z+1];
                uint32_t lightLevelSouth = lightLevel[x][y][z-1];
                uint32_t lightLevelWest = lightLevel[x+1][y][z];
                uint32_t lightLevelEast = lightLevel[x-1][y][z];

                lightLevelsCorrected[x][y][z] = std::max({lightLevelUp, lightLevelSouth, lightLevelNorth, lightLevelEast, lightLevelWest});
            }
        }
    }
    updateLightTexture();
}

void Chunk::makeLightTexture() {
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_3D, textureID);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, 16, 16, 16, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
}

void Chunk::updateLightTexture() {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, textureID);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, 16, 16, 16, GL_RED, GL_UNSIGNED_BYTE, lightLevelsCorrected);
}
