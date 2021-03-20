//
// Created by quartzy on 2/18/21.
//

#ifndef MCCPP_TEXTUREATLAS_H
#define MCCPP_TEXTUREATLAS_H

#include <unordered_map>
#include "GL/glew.h"

class TextureAtlas {
private:
    GLuint textureId;
    std::unordered_map<std::string, uint32_t> textureIndexes;
    uint32_t textureCount = 0;
public:
    TextureAtlas() = default;

    ~TextureAtlas() = default;

    void load(const char *dir_path, uint32_t size);

    GLuint getTextureId() const;

    uint32_t getTextureCount() const;

    uint32_t getTextureFromName(const char* name);
};


#endif //MCCPP_TEXTUREATLAS_H
