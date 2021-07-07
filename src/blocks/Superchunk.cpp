//
// Created by quartzy on 2/17/21.
//

#include <exception>
#include <algorithm>
#include <iostream>
#include "Superchunk.h"
#include "../MCCPP.h"
#include "../util/Log.h"

Superchunk::Superchunk(ShaderProgram &shaderProgram, std::string &lightingSource, GLuint renderDistance)
        : shaderProgram(shaderProgram), renderDistance(renderDistance) {
//    chunks = std::vector<Chunk>(renderDistance*renderDistance*VERTICAL_CHUNKS, {shaderProgram, nullptr, nullptr, nullptr,
//                                nullptr, nullptr, nullptr});
    xOffset = 0;
    zOffset = 0;

    chunkArr = std::make_unique<Chunk[]>(renderDistance*renderDistance*VERTICAL_CHUNKS);
    chunks = std::vector<Chunk*>();
    chunks.reserve(renderDistance*renderDistance*VERTICAL_CHUNKS);
    for (int x = 0; x < VERTICAL_CHUNKS; ++x) {
        for (int z = 0; z < renderDistance; ++z) {
            for (int y = 0; y < renderDistance; ++y) {
                chunks.emplace_back(new(chunkArr.get() + (x*renderDistance*renderDistance + z*renderDistance + y)) Chunk(&shaderProgram, nullptr, nullptr,
                                                                                                                         nullptr, nullptr, nullptr,
                                                                                                                         nullptr, x, y, z));
            }
        }
    }
//    updateOffset(xOffset-1, zOffset);

//    chunk_list = std::unordered_map<chunk_pos, Chunk*>{};
    Chunk::init();

    lightingSource.insert(129, "#define RENDER_DISTANCE 20\n#define VERTICAL_SIZE 16\n");

    const char* str = lightingSource.c_str();
    lightingShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(lightingShader, 1, &str, nullptr);
    glCompileShader(lightingShader);
    GLint compile_ok = -1;
    glGetShaderiv(lightingShader, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok!=GL_TRUE) {
        GLint maxLength = 2048;
        glGetShaderiv(lightingShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(lightingShader, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(lightingShader); // Don't leak the shader.
        CORE_ERROR("Error while compiling compute shader: {}", errorLog.data());
        return;
    }
    
    lightingProgram = glCreateProgram();
    glAttachShader(lightingProgram, lightingShader);
    glLinkProgram(lightingProgram);

    GLint link_ok = 0;
    glGetProgramiv(lightingProgram, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        GLint maxLength = 2048;
        glGetProgramiv(lightingProgram, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(lightingProgram, maxLength, &maxLength, &errorLog[0]);

        glDeleteProgram(lightingProgram);
        CORE_ERROR("Error in glLinkProgram: {}", errorLog.data());
        return;
    }

    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    CORE_INFO("max global (total) work group counts x:{} y:{} z:{}", work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

    int work_grp_size[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

    CORE_INFO("max local (in one shader) work group sizes x:{} y:{} z:{}", work_grp_size[0], work_grp_size[1], work_grp_size[2]);

    GLint work_grp_inv = 0;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    CORE_INFO("max local work group invocations {}", work_grp_inv);

    glUseProgram(lightingProgram);
    glGenBuffers(1, &lightLevelBufId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightLevelBufId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, renderDistance * renderDistance * VERTICAL_CHUNKS * 16 * 16 * 16, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lightLevelBufId);

    glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R8, GL_RED, GL_UNSIGNED_INT, nullptr);

    LightSource lightSource[1];
    lightSource[0] = LightSource{5, 0, 5, 16};

    glGenBuffers(1, &lightSourcesBufId);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightSourcesBufId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightSource)*1, &lightSource[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, lightSourcesBufId);

    updateLighting();
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

    Chunk* chunk = getOrMakeChunk(cx, cy, cz);
    if (!chunk)return;

    chunk->set(x1, y1, z1, type);

    updateNearbyChunks(cx, cy, cz);
}

void Superchunk::render(glm::mat4 vp) {
    shaderProgram.bind();
    for(int i = 0;i<chunks.size();i++){
        Chunk* chunk = chunks[i];
        if (chunk) {
            if (chunk->getPosX()==INT32_MAX || chunk->getPosY()==INT32_MAX || chunk->getPosZ()==INT32_MAX)continue;
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
    if (!chunk)return;
    if (chunk->getPosX()!=INT32_MAX && chunk->getPosY()!=INT32_MAX && chunk->getPosZ()!=INT32_MAX){
        chunk->makeNeedUpdate(getChunkAt(x+1, y, z), getChunkAt(x-1, y, z),
                                            getChunkAt(x, y+1, z), getChunkAt(x, y-1, z),
                                            getChunkAt(x, y, z+1), getChunkAt(x, y, z-1));
    }
}

void Superchunk::updateChunk(Chunk *chunk) {
    if (chunk==nullptr)return;
    int32_t cx = chunk->getArrayPosX(), cy = chunk->getArrayPosY(), cz = chunk->getArrayPosZ();
    updateChunk(cx, cy, cz);
    updateChunk(cx+1, cy, cz);
    updateChunk(cx-1, cy, cz);
    updateChunk(cx, cy+1, cz);
    updateChunk(cx, cy-1, cz);
    updateChunk(cx, cy, cz+1);
    updateChunk(cx, cy, cz-1);
}

Chunk *Superchunk::getOrMakeChunk(int32_t cx, int32_t cy, int32_t cz) {
    Chunk* chunk = getChunkAt(cx, cy, cz);
    if (!chunk)return nullptr;
    if (chunk->getPosX()!=cx || chunk->getPosY()!=cy || chunk->getPosZ()!=cz){
        chunk->makeNeedUpdate(getChunkAt(cx + 1, cy, cz), getChunkAt(cx - 1, cy, cz),
                              getChunkAt(cx, cy + 1, cz), getChunkAt(cx, cy - 1, cz),
                              getChunkAt(cx, cy, cz - 1), getChunkAt(cx, cy, cz - 1));
        chunk->setPosX(cx);
        chunk->setPosY(cy);
        chunk->setPosZ(cz);
    }

    return chunk;
}

void Superchunk::updateNearbyChunks(int32_t cx, int32_t cy, int32_t cz) {
    updateChunk(cx, cy, cz);
    updateChunk(cx+1, cy, cz);
    updateChunk(cx-1, cy, cz);
    updateChunk(cx, cy+1, cz);
    updateChunk(cx, cy-1, cz);
    updateChunk(cx, cy, cz+1);
    updateChunk(cx, cy, cz-1);
}

void Superchunk::updateLighting() {
    glUseProgram(lightingProgram);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightLevelBufId);
    glDispatchCompute(1, 1, 1);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightLevelBufId);
    int32_t* buf = (int32_t*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    CORE_INFO("Element 0: {}", buf[1 * 16 * 16]);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void Superchunk::cleanup() {
    chunks.clear();
    shaderProgram.cleanup();

    glDeleteShader(lightingShader);
    glDeleteProgram(lightingProgram);
    glDeleteBuffers(1, &lightLevelBufId);
    glDeleteBuffers(1, &lightSourcesBufId);
}

void Superchunk::updateOffset(int32_t newOffsetX, int32_t newOffsetZ) {
    /*newOffsetX += xOffset;
    newOffsetZ += zOffset;
    int32_t offsetXDiff = xOffset - newOffsetX;
    int32_t offsetZDiff = zOffset - newOffsetZ;

//    std::vector<Chunk*> movedChunks{};
//    movedChunks.reserve(abs(offsetXDiff*renderDistance) + abs(offsetZDiff*renderDistance));
    if (offsetXDiff<0){
        for (int x = 0; x < renderDistance+offsetXDiff; ++x) {
            for (int z = 0; z < renderDistance; ++z) {
                for (int y = 0; y < VERTICAL_CHUNKS; ++y) {
//                    auto& chunk1 = getChunkAtRef(x, y, z);
//                    auto& chunk2 = getChunkAtRef(x-offsetXDiff, y, z);
                    try{
                        auto chunk1 = chunks.at((x * renderDistance * renderDistance) + (z * renderDistance) + (y));
                        auto chunk2 = chunks.at(((x-offsetXDiff) * renderDistance * renderDistance) + (z * renderDistance) + (y));
//                        std::swap(chunk1->posX, chunk2->posX);
//                        std::swap(chunk1->posY, chunk2->posY);
//                        std::swap(chunk1->posZ, chunk2->posZ);
                        chunks.at((x * renderDistance * renderDistance) + (z * renderDistance) + (y)) = chunk2;
                        chunks.at(((x-offsetXDiff) * renderDistance * renderDistance) + (z * renderDistance) + (y)) = chunk1;
                        int a = 3;
                    }catch (std::exception& e){
                        std::cout << e.what() << "\n";
                    }
                }
            }
        }


//        for (Chunk* chunk : chunks){
//            int32_t relativePosX = chunk->posX - newOffsetX;
//            if (relativePosX<0){
//                chunk->posX = newOffsetX + ((renderDistance-1) - (chunk->posX - xOffset));
//            }
//
//        }
    }else if(offsetXDiff>0){
        for (Chunk* chunk : chunks){
            int32_t relativePosX = chunk->posX - newOffsetX;
            if (relativePosX>renderDistance-1){
                chunk->posX = newOffsetX + renderDistance - relativePosX;
            }
        }
    }
    if (offsetZDiff<0){
        for (Chunk* chunk : chunks){
            int32_t relativePosZ = chunk->posZ - newOffsetZ;
            if (relativePosZ<0){
                chunk->posZ = newOffsetZ + ((renderDistance-1) - (chunk->posZ - zOffset));
            }
        }
    }else if(offsetZDiff>0){
        for (Chunk* chunk : chunks){
            int32_t relativePosZ = chunk->posZ - newOffsetZ;
            if (relativePosZ>renderDistance-1){
                chunk->posZ = newOffsetZ + renderDistance - relativePosZ;
            }
        }
    }*/

    xOffset = newOffsetX;
    zOffset = newOffsetZ;
    CORE_INFO("Offset is now {} {}", xOffset, zOffset);
}

GLint Superchunk::getRenderDistance() const {
    return renderDistance;
}
