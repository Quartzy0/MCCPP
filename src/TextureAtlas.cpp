//
// Created by quartzy on 2/18/21.
//

#include "TextureAtlas.h"

#include <iostream>

#include <filesystem>
#include "lodepng/lodepng.h"
#include "util/Log.h"

void TextureAtlas::load(const char *dir_path, uint32_t size) {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    for (const auto & entry : std::filesystem::directory_iterator(dir_path)){
        std::string filename = std::string(entry.path().filename().c_str());
        std::string supportedExtension = ".png";
        if (filename.length() >= supportedExtension.length()) {
            if (filename.compare(filename.length() - supportedExtension.length(),
                                                supportedExtension.length(), supportedExtension))continue;
        } else {
            continue;
        }
        textureCount++;
    }

//    glTextureStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, size, size, textureCount);

    GLubyte* textureArrayBuffer = new GLubyte[textureCount*size*size*4];
    uint32_t i = 0;
    for (const auto & entry : std::filesystem::directory_iterator(dir_path)){
        std::string filename = std::string{entry.path().filename().c_str()};
        std::string supportedExtension = ".png";
        if (filename.length() >= supportedExtension.length()) {
            if (filename.compare(filename.length() - supportedExtension.length(),
                                 supportedExtension.length(), supportedExtension))continue;
        } else {
            continue;
        }

        std::vector<unsigned char> image;
        unsigned width, height;

        //decode
        unsigned error = lodepng::decode(image, width, height, entry.path(), LCT_RGBA, 8);

        if(error){
            CORE_ERROR("Encode error {}: {}", error, lodepng_error_text(error));
            i++;
            continue;
        }
        if (width>height){
            CORE_ERROR("Width is bigger than the height");
            i++;
            continue;
        }
        if (width>size){
            CORE_WARN("Texture {} too big", filename);
        }

        textureIndexes[filename] = i;
        const uint32_t ratio = size/width;
        if (ratio>1){
            for (int y = 0; y < size; ++y) {
                for (int x = 0; x < size; ++x) {
                    textureArrayBuffer[((i*size*size)+size*y+x)*4] = image[((width*(y/ratio)+(x/ratio)))*4];
                    textureArrayBuffer[((i*size*size)+size*y+x)*4+1] = image[((width*(y/ratio)+(x/ratio)))*4+1];
                    textureArrayBuffer[((i*size*size)+size*y+x)*4+2] = image[((width*(y/ratio)+(x/ratio)))*4+2];
                    textureArrayBuffer[((i*size*size)+size*y+x)*4+3] = image[((width*(y/ratio)+(x/ratio)))*4+3];
                }
            }
            i++;
            continue;
        }
        for (int j = 0; j < size * size * 4; ++j) {
            textureArrayBuffer[(i*size*size*4)+j] = image[j];
        }
        i++;
    }

//    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, size, size, textureCount, GL_RGBA, GL_UNSIGNED_BYTE, textureArrayBuffer);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                 0,                 // mipmap level
                 GL_RGBA8,          // gpu texel format
                 size,             // width
                 size,             // height
                 textureCount,             // depth
                 0,                 // border
                 GL_RGBA,      // cpu pixel format
                 GL_UNSIGNED_BYTE,  // cpu pixel coord type
                 textureArrayBuffer);
    delete[] textureArrayBuffer;
}

GLuint TextureAtlas::getTextureId() const {
    return textureId;
}

uint32_t TextureAtlas::getTextureCount() const {
    return textureCount;
}

uint32_t TextureAtlas::getTextureFromName(const char* name){
    return textureIndexes[std::string{name} + ".png"];
}
