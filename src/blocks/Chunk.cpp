//
// Created by quartzy on 2/16/21.
//

#include <cstring>
#include <glm/common.hpp>
#include "Chunk.h"

#include "../graphics/Vertex.h"

Chunk::Chunk(ShaderProgram *shaderProgram1, Chunk *chunkPosX, Chunk *chunkNegX, Chunk *chunkPosY, Chunk *chunkNegY,
             Chunk *chunkPosZ, Chunk *chunkNegZ, int32_t arrayPosX, int32_t arrayPosY, int32_t arrayPosZ)
        : shaderProgram(shaderProgram1), chunkPosX(chunkPosX), chunkNegX(chunkNegX), chunkPosY(chunkPosY),
          chunkNegY(chunkNegY), chunkPosZ(chunkPosZ), chunkNegZ(chunkNegZ), arrayPosX(arrayPosX), arrayPosY(arrayPosY), arrayPosZ(arrayPosZ) {
    std::memset(block, 0, sizeof(block));
    elements = 0;
    changed = true;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    posX = INT32_MAX;
    posY = INT32_MAX;
    posZ = INT32_MAX;
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
    glBindVertexArray(vao);
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

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    shaderProgram->specifyVertexAttributes();

//    shaderProgram.setUniformLightLevels(&lightLevel[0][0][0], CX*CY*CZ);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_3D, bufferId);
    shaderProgram->setUniform("lightLevel", 1);

    glDrawArrays(GL_LINES, 0, elements);
}

void Chunk::cleanup() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
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

void Chunk::init() {

}

int32_t Chunk::getArrayPosX() const {
    return arrayPosX;
}

int32_t Chunk::getArrayPosY() const {
    return arrayPosY;
}

int32_t Chunk::getArrayPosZ() const {
    return arrayPosZ;
}
