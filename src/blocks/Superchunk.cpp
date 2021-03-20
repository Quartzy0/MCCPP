//
// Created by quartzy on 2/17/21.
//

#include <cstring>
#include "Superchunk.h"
#include "../MCCPP.h"

Superchunk::Superchunk(ShaderProgram &shaderProgram) : shaderProgram(shaderProgram) {
    std::memset(chunk, 0, sizeof(chunk));
}

Superchunk::~Superchunk() {
    for (int x = 0;x<SCX;x++){
        for (int y = 0;y<SCY;y++){
            for (int z = 0; z < SCZ; ++z) {
                delete chunk[x][y][z];
            }
        }
    }
}

uint16_t Superchunk::get(int32_t x, int32_t y, int32_t z) {
    int cx = x / CX;
    int cy = y / CY;
    int cz = z / CZ;

    int x1 = x % CX;
    int y1 = y % CY;
    int z1 = z % CZ;

    if (x1 < 0){
        x1+=16;
    }
    if (y1 < 0){
        y1+=16;
    }
    if (z1 < 0){
        z1+=16;
    }

    if(!chunk[cx][cy][cz])
        return 0;
    else
        return chunk[cx][cy][cz]->get(x1, y1, z1);
}

void Superchunk::set(int32_t x, int32_t y, int32_t z, uint16_t type) {
    int cx = x / CX;
    int cy = y / CY;
    int cz = z / CZ;

    int x1 = x % CX;
    int y1 = y % CY;
    int z1 = z % CZ;

    if (x1 < 0){
        x1+=16;
    }
    if (y1 < 0){
        y1+=16;
    }
    if (z1 < 0){
        z1+=16;
    }

    if(!chunk[cx][cy][cz])
        chunk[cx][cy][cz] = new Chunk(shaderProgram);

    chunk[cx][cy][cz]->set(x1, y1, z1, type);
}

void Superchunk::render(glm::mat4 vp) {
    for (int x = 0; x < SCX; x++) {
        for (int y = 0; y < SCY; y++) {
            for (int z = 0; z < SCZ; z++) {
                if (chunk[x][y][z]) {
                    glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(x * CX, y * CY, z * CZ));
                    glm::mat4 mvp = vp * model;

#ifdef CHUNK_CULLING
                    // Is this chunk on the screen?
                    glm::vec4 center = mvp * glm::vec4(CX / 2, CY / 2, CZ / 2, 1);

                    center.x /= center.w;
                    center.y /= center.w;

                    // If it is behind the camera, don't bother drawing it
                    if(center.z < -CY / 2)
                        continue;

                    // If it is outside the screen, don't bother drawing it
                    if(fabsf(center.x) > 1 + fabsf(CY * 2 / center.w) || fabsf(center.y) > 1 + fabsf(CY * 2 / center.w))
                        continue;
#endif

                    shaderProgram.setUniform("mvp", mvp);
                    chunk[x][y][z]->render();
                }
            }
        }
    }
}
