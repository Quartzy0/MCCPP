//
// Created by quartzy on 2/17/21.
//

#ifndef MCCPP_SUPERCHUNK_H
#define MCCPP_SUPERCHUNK_H

//#define CHUNK_CULLING
//#define RENDER_DISTANCE 20
#define VERTICAL_CHUNKS 16

#include "Chunk.h"
#include "../util/Log.h"

union chunk_pos
{
    struct
    {
        int32_t x = 0;
        int32_t y = 0;
        int32_t z = 0;
    };
    int32_t data[3];

    bool operator==(const chunk_pos& other) const noexcept
    {
        return other.x == x && other.y == y && other.z == z;
    }
};

namespace std
{
    template<> struct hash<chunk_pos>
    {
        inline size_t operator()(chunk_pos position) const noexcept
        {
            size_t hash = 17;
            hash = 31 * hash + position.x;
            hash = 31 * hash + position.y;
            hash = 31 * hash + position.z;
            return hash;
        }
    };

}

struct LightSource{
    int32_t x, y, z;
    uint32_t lightLevel;
};

class Superchunk {
private:
    ShaderProgram &shaderProgram;

//    std::unordered_map<chunk_pos, Chunk*, std::hash<chunk_pos>> chunk_list;
    std::vector<Chunk*> chunks;
    std::unique_ptr<Chunk[]> chunkArr;
    int32_t xOffset, zOffset;

    inline Chunk *getChunkAt(int32_t x, int32_t y, int32_t z)
    {
        if(!(x >= xOffset && z >= zOffset && x < (xOffset + renderDistance) && z < (zOffset + renderDistance) && y>=0))return nullptr;

        int32_t x1 = (x) % renderDistance;
        int32_t z1 = (z) % renderDistance;

        x1+=renderDistance * (x1 < 0);
        z1+=renderDistance * (z1 < 0);

        return chunks.at((x1 * renderDistance * renderDistance) + (z1 * renderDistance) + (y));
    }

    inline Chunk *&getChunkAtRef(int32_t x, int32_t y, int32_t z)
    {
        int32_t x1 = (x) % renderDistance;
        int32_t z1 = (z) % renderDistance;

        x1+=renderDistance * (x1 < 0);
        z1+=renderDistance * (z1 < 0);

        return chunks.at((x1 * renderDistance * renderDistance) + (z1 * renderDistance) + (y));
    }

    GLuint lightingShader, lightingProgram;
    GLuint lightLevelBufId, lightSourcesBufId;

    GLint renderDistance;

    void updateChunk(int32_t x, int32_t y, int32_t z);
    void updateChunk(Chunk* chunk);
public:
    void updateOffset(int32_t newOffsetX, int32_t newOfsserZ);
    void updateNearbyChunks(int32_t x, int32_t y, int32_t z);

    GLint getRenderDistance() const;

    //If you are using this to set multiple blocks within a chunk, remember to call the updateNearbyChunks() method to update all of the nearby chunks
    Chunk* getOrMakeChunk(int32_t x, int32_t y, int32_t z);

    Superchunk(ShaderProgram &shaderProgram, std::string &lightingSource, GLuint renderDistance);

    ~Superchunk() = default;

    Block* get(int32_t x, int32_t y, int32_t z);

    void set(int32_t x, int32_t y, int32_t z, Block* type);

    void render(glm::mat4 vp);

    void updateLighting();

    void cleanup();
};


#endif //MCCPP_SUPERCHUNK_H
