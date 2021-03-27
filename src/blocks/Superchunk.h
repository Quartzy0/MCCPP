//
// Created by quartzy on 2/17/21.
//

#ifndef MCCPP_SUPERCHUNK_H
#define MCCPP_SUPERCHUNK_H

#define SCX 8
#define SCY 16
#define SCZ 8

#define CHUNK_CULLING

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
            //std::string_view v{(const char*)position.data};
            //return std::hash<std::string_view>{}(v);
        }
    };

}

class Superchunk {
private:
    //std::vector<Chunk *> chunks;
    //std::unordered_map<Position, Chunk*> chunks;
    ShaderProgram &shaderProgram;

    std::unordered_map<chunk_pos, Chunk*, std::hash<chunk_pos>> chunk_list;
    //size_t a = std::hash<chunk_pos>{}({1,1,1,0});

//    struct HashElement {
//        uint64_t hash;
//        Chunk* ptr[SCY];
//    };
//
//    std::vector<HashElement> chunk_list;

    Chunk *getChunkAt(int32_t x, int32_t y, int32_t z)
    {
        /*if (y<0)return nullptr;
        //return (long)x & 4294967295L | ((long)z & 4294967295L) << 32;
        uint64_t hash_value = ((uint64_t)x << 32) | (uint64_t)z;
        for (auto& element : chunk_list)
        {
            if (element.hash == hash_value)
            {
                return element.ptr[y];
            }
        }
        return nullptr;*/
        auto chunkFound = chunk_list.find({x,y,z});
        if (chunkFound==chunk_list.end()){
            return nullptr;
        }
        return (*chunkFound).second;
    }



    Chunk* last;

    void updateChunk(int32_t x, int32_t y, int32_t z);
public:

    Superchunk(ShaderProgram& shaderProgram);

    ~Superchunk();

    Block* get(int32_t x, int32_t y, int32_t z);

    void set(int32_t x, int32_t y, int32_t z, Block* type);

    Chunk* getChunk(int32_t x, int32_t y, int32_t z);

    void render(glm::mat4 vp);
};


#endif //MCCPP_SUPERCHUNK_H
