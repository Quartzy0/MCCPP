//
// Created by quartzy on 2/16/21.
//

#include <cstring>
#include <glm/common.hpp>
#include "Chunk.h"

#include "../graphics/Vertex.h"

Chunk::Chunk(ShaderProgram &shaderProgram1) : shaderProgram(shaderProgram1){
    std::memset(block, 0, sizeof(block));
    elements = 0;
    changed = true;
    glGenBuffers(1, &vbo);
}

Chunk::~Chunk() {
    cleanup();
}

uint16_t Chunk::get(uint32_t x, uint32_t y, uint32_t z) {
    return block[x][y][z];
}

void Chunk::set(uint32_t x, uint32_t y, uint32_t z, uint16_t type) {
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
                uint16_t type = block[x][y][z];

                if (!type || !(x==0 || !block[x-1][y][z])){
                    visible = false;
                    continue;
                }

                // View from negative x
                if(visible && type == block[x][y][z - 1]) {
                    vertex[i-1].z = z + 1;
                }else {
                    vertex[i++] = Vertex(x, y, z, type);
                    vertex[i++] = Vertex(x, y+1, z+1, type);
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
                uint16_t type = block[x][y][z];

                if (!type || !(x==CX-1 || !block[x+1][y][z])){
                    visible = false;
                    continue;
                }

                // View from positive x
                if(visible && type == block[x][y][z - 1]) {
                    vertex[i-1].z = z + 1;
                }else{
                    vertex[i++] = Vertex(x+1, y, z, type);
                    vertex[i++] = Vertex(x+1, y+1, z+1, type);
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
                uint16_t type = block[x][y][z];

                if (!type || !(y==0 || !block[x][y-1][z])){
                    visible = false;
                    continue;
                }

                // View from negative y
                if(visible && type == block[x][y][z - 1]) {
                    vertex[i-1].z = z + 1;
                }else{
                    vertex[i++] = Vertex(x, y, z, -type);
                    vertex[i++] = Vertex(x+1, y, z+1, -type);
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
                uint16_t type = block[x][y][z];

                if (!type || !((y==CY-1 || !block[x][y+1][z]))){
                    visible = false;
                    continue;
                }

                // View from positive y
                if(visible && type == block[x][y][z - 1]) {
                    vertex[i-1].z = z + 1;
                }else {
                    vertex[i++] = Vertex(x, y+1, z, -type);
                    vertex[i++] = Vertex(x+1, y+1, z+1, -type);
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
                uint16_t type = block[x][y][z];

                if (!type || !(z==0 || !block[x][y][z-1])){
                    visible = false;
                    continue;
                }

                // View from negative z
                if(visible && type == block[x][y - 1][z]) {
                    vertex[i-1].y = y + 1;
                }else {
                    vertex[i++] = Vertex(x, y, z, type);
                    vertex[i++] = Vertex(x+1, y+1, z, type);
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
                uint16_t type = block[x][y][z];

                if (!type || !(z==CZ-1 || !block[x][y][z+1])){
                    visible = false;
                    continue;
                }

                // View from positive z
                if(visible && type == block[x][y - 1][z]) {
                    vertex[i-1].y = y + 1;
                }else {
                    vertex[i++] = Vertex(x, y, z+1, type);
                    vertex[i++] = Vertex(x+1, y+1, z+1, type);
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
//    glFrontFace(GL_CW);
//    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    shaderProgram.bind();
    shaderProgram.specifyVertexAttributes();

    glDrawArrays(GL_LINES, 0, elements);
}

void Chunk::cleanup() {
    glDeleteBuffers(1, &vbo);
}
