//
// Created by quartzy on 2/17/21.
//

#ifndef MCCPP_SUPERCHUNK_H
#define MCCPP_SUPERCHUNK_H

//#define CHUNK_CULLING

#include "Chunk.h"

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

class Superchunk {
private:
    ShaderProgram &shaderProgram;

    std::unordered_map<chunk_pos, Chunk*, std::hash<chunk_pos>> chunk_list;

    Chunk *getChunkAt(int32_t x, int32_t y, int32_t z)
    {
        auto chunkFound = chunk_list.find({x,y,z});
        if (chunkFound==chunk_list.end()){
            return nullptr;
        }
        return (*chunkFound).second;
    }

    void updateChunk(int32_t x, int32_t y, int32_t z);
public:
    void updateNearbyChunks(int32_t x, int32_t y, int32_t z);

    //If you are using this to set multiple blocks within a chunk, remember to call the updateChunk() method to update all of the nearby chunks
    Chunk* getOrMakeChunk(int32_t x, int32_t y, int32_t z);

    Superchunk(ShaderProgram& shaderProgram);

    ~Superchunk();

    Block* get(int32_t x, int32_t y, int32_t z);

    void set(int32_t x, int32_t y, int32_t z, Block* type);

    void render(glm::mat4 vp);
};


#endif //MCCPP_SUPERCHUNK_H
