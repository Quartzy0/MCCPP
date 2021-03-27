//
// Created by quartzy on 2/17/21.
//

#include <cstring>
#include <exception>
#include <algorithm>
#include <execution>
#include <iostream>
#include "Superchunk.h"
#include "../MCCPP.h"

Superchunk::Superchunk(ShaderProgram &shaderProgram) : shaderProgram(shaderProgram) {
    chunk_list = std::unordered_map<chunk_pos, Chunk*>{};
}

Superchunk::~Superchunk() {
    for (auto chunk : chunk_list){
        delete chunk.second;
    }
}

Block* Superchunk::get(int32_t x, int32_t y, int32_t z) {
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

    Chunk* chunk = getChunkAt(cx, cy, cz);
    if(!chunk)
        return nullptr;
    else
        return chunk->get(x1, y1, z1);
}

void Superchunk::set(int32_t x, int32_t y, int32_t z, Block* type) {
    int cx = x / CX;
    int cy = y / CY;
    int cz = z / CZ;

    int x1 = x % CX;
    int y1 = y % CY;
    int z1 = z % CZ;

    if (x1<0){
        x1+=16;
        --cx;
    }
    if (y1<0){
        y1+=16;
        --cy;
    }
    if (z1<0){
        z1+=16;
        --cz;
    }

    Chunk* chunk = getChunkAt(cx, cy, cz);
    if(!chunk){
        Chunk* newChunk = new Chunk(shaderProgram, getChunkAt(cx+1, cy, cz), getChunkAt(cx-1, cy, cz),
                                      getChunkAt(cx, cy+1, cz), getChunkAt(cx, cy-1, cz),
                                      getChunkAt(cx, cy, cz-1), getChunkAt(cx, cy, cz-1));
        newChunk->setPosX(cx);
        newChunk->setPosY(cy);
        newChunk->setPosZ(cz);
        newChunk->set(x1, y1, z1, type);
        chunk_list[{cx,cy,cz}] = newChunk;
    }else{
        chunk->set(x1, y1, z1, type);
    }

    if (getChunkAt(cx+1, cy, cz))updateChunk(cx+1, cy, cz);
    if (getChunkAt(cx-1, cy, cz))updateChunk(cx-1, cy, cz);
    if (getChunkAt(cx, cy+1, cz))updateChunk(cx, cy+1, cz);
    if (getChunkAt(cx, cy-1, cz))updateChunk(cx, cy-1, cz);
    if (getChunkAt(cx, cy, cz+1))updateChunk(cx, cy, cz+1);
    if (getChunkAt(cx, cy, cz-1))updateChunk(cx, cy, cz-1);
}

void Superchunk::render(glm::mat4 vp) {
    for(auto chunkE : chunk_list){
        Chunk* chunk = chunkE.second;
        if (chunk) {
            glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(chunk->getPosX() * CX, chunk->getPosY() * CY, chunk->getPosZ() * CZ));

#ifdef CHUNK_CULLING
            glm::mat4 mvp = vp * model;

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

            shaderProgram.setUniform("vp", vp);
            shaderProgram.setUniform("m", model);
            chunk->render();
        }
    }
}

void Superchunk::updateChunk(int32_t x, int32_t y, int32_t z) {
    Chunk* chunk = getChunkAt(x,y,z);
    if (chunk){
        chunk->makeNeedUpdate(getChunkAt(x+1, y, z), getChunkAt(x-1, y, z),
                                            getChunkAt(x, y+1, z), getChunkAt(x, y-1, z),
                                            getChunkAt(x, y, z+1), getChunkAt(x, y, z-1));
    }
}

Chunk *Superchunk::getChunk(int32_t x, int32_t y, int32_t z) {
    /*if (last != nullptr){
        if (last->getPosX()==x && last->getPosY()==y && last->getPosZ()==z){
            return last;
        }
    }
//    auto foundChunk = std::find_if(std::execution::seq, chunks.begin(), chunks.end(), [x,y,z](Chunk* chunkIn){
//        return chunkIn->getPosX()==x && chunkIn->getPosY()==y && chunkIn->getPosZ()==z;
//    });
    for (Chunk* chunk : chunks){
        if (chunk->getPosX()==x && chunk->getPosY()==y && chunk->getPosZ()==z){
            last = chunk;
            return chunk;
        }
    }
    return nullptr;*/
}
