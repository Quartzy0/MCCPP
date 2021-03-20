//
// Created by quartzy on 2/16/21.
//

#ifndef MCCPP_CHUNK_H
#define MCCPP_CHUNK_H

#include "../graphics/ShaderProgram.h"
#include <cstdint>
#include <GL/glew.h>

#define CX 16
#define CY 16
#define CZ 16


class Chunk {
private:
    uint16_t block[CX][CY][CZ];
    GLuint vbo;
    uint32_t elements;
    bool changed;
    ShaderProgram& shaderProgram;
public:
    Chunk(ShaderProgram& shaderProgram1);

    ~Chunk();

    uint16_t get(uint32_t x, uint32_t y, uint32_t z);

    void set(uint32_t x, uint32_t y, uint32_t z, uint16_t type);

    void update();

    void render();

    void cleanup();
};


#endif //MCCPP_CHUNK_H
