//
// Created by quartzy on 2/18/21.
//

#ifndef MCCPP_TEXTUREATLAS_H
#define MCCPP_TEXTUREATLAS_H

#include <unordered_map>
#include <vector>
#include "GL/glew.h"

class TextureAtlas {
private:
    GLuint textureId;
    uint32_t size;
    std::unordered_map<std::string, uint32_t> textureIndexes;
    std::unordered_map<uint32_t, uint8_t> frameTimes;
    std::unordered_map<uint32_t, std::vector<uint8_t>> frames;
    std::vector<uint32_t> animatedTextures;
    uint32_t textureCount = 0;
public:
    TextureAtlas() = default;

    ~TextureAtlas() = default;

    void load(const char *dir_path, uint32_t size);

    GLuint getTextureId() const;

    uint32_t getTextureCount() const;

    uint32_t getTextureFromName(const char* name);

    uint8_t getFrameTime(uint32_t textureIdOriginal);

    std::vector<uint8_t> getFrames(uint32_t textureIdOriginal);

    bool isAnimated(uint32_t textureIdOriginal);

    void tick(uint32_t tick);
};


#endif //MCCPP_TEXTUREATLAS_H
