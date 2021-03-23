//
// Created by quartzy on 2/18/21.
//

#include "TextureAtlas.h"

#include "nlohmann/json.hpp"

#include <fstream>

#include <filesystem>
#include "lodepng/lodepng.h"
#include "util/Log.h"

std::string readFile_1(const char* filePath){
    std::fstream fstream;
    fstream.open(filePath);
    std::ostringstream oss;
    oss << fstream.rdbuf();
    fstream.close();
    return oss.str();
}

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
    this->size = size;

//    glTextureStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, size, size, textureCount);

    std::vector<GLubyte> textureArrayBuffer{};
    textureArrayBuffer.reserve(textureCount*size*size*4);

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

        if (height>width){
            uint32_t animationLen = height/width;

            std::stringstream ss;
            ss << dir_path << "/" << filename << ".mcmeta";
            const auto name = ss.str();
            std::string jsonRead = readFile_1(name.c_str());

            auto jsonParsed = nlohmann::json::parse(jsonRead);
            if (jsonParsed["animation"].find("frametime") != jsonParsed["animation"].end()){
                frameTimes[i] = jsonParsed["animation"]["frametime"].get<uint8_t>();
            }else{
                frameTimes[i] = 1;
            }

            if (jsonParsed["animation"].find("frames") != jsonParsed["animation"].end()){
                frames[i] = std::vector<uint8_t>{};
                frames[i].resize(jsonParsed["animation"]["frames"].get<std::vector<uint8_t>>().size());
                std::memcpy(frames[i].data(), jsonParsed["animation"]["frames"].get<std::vector<uint8_t>>().data(), jsonParsed["animation"]["frames"].get<std::vector<uint8_t>>().size());
            }else{
                frames[i] = std::vector<uint8_t>{};
                frames[i].resize(animationLen);
                for (int j = 0; j < animationLen; ++j) {
                    frames[i][j] = j;
                }
            }


            textureCount+=animationLen;
            textureArrayBuffer.reserve(textureCount*size*size*4);
            const uint32_t ratio = size/width;

            if (ratio>1){
                for (int y = 0; y < size; ++y) {
                    for (int x = 0; x < size; ++x) {
                        textureArrayBuffer.push_back(image[((width*(y/ratio)+(x/ratio)))*4]);
                        textureArrayBuffer.push_back(image[((width*(y/ratio)+(x/ratio)))*4+1]);
                        textureArrayBuffer.push_back(image[((width*(y/ratio)+(x/ratio)))*4+2]);
                        textureArrayBuffer.push_back(image[((width*(y/ratio)+(x/ratio)))*4+3]);
                    }
                }
                textureIndexes[filename] = i;
                animatedTextures.push_back(i++);

                for (int j = 0; j < animationLen; ++j) {
                    for (int y = 0; y < size; ++y) {
                        for (int x = 0; x < size; ++x) {
                            textureArrayBuffer.push_back(image[(j*width*width*4)+((width*(y/ratio)+(x/ratio)))*4]);
                            textureArrayBuffer.push_back(image[(j*width*width*4)+((width*(y/ratio)+(x/ratio)))*4+1]);
                            textureArrayBuffer.push_back(image[(j*width*width*4)+((width*(y/ratio)+(x/ratio)))*4+2]);
                            textureArrayBuffer.push_back(image[(j*width*width*4)+((width*(y/ratio)+(x/ratio)))*4+3]);
                        }
                    }
                    textureIndexes[filename + "-" + std::to_string(j)] = i++;
                }
            }else{
                for (int k = 0; k < size * size * 4; ++k) {
                    textureArrayBuffer.push_back(image[k]);
                }
                textureIndexes[filename] = i;
                animatedTextures.push_back(i++);

                for (int j = 0; j < animationLen; ++j) {
                    for (int k = 0; k < size * size * 4; ++k) {
                        textureArrayBuffer.push_back((j*size*size*4)+image[k]);
                    }
                    textureIndexes[filename + "-" + std::to_string(j)] = i++;
                }
            }
        }else{
            textureIndexes[filename] = i;
            const uint32_t ratio = size/width;
            if (ratio>1){
                for (int y = 0; y < size; ++y) {
                    for (int x = 0; x < size; ++x) {
                        textureArrayBuffer.push_back(image[((width*(y/ratio)+(x/ratio)))*4]);
                        textureArrayBuffer.push_back(image[((width*(y/ratio)+(x/ratio)))*4+1]);
                        textureArrayBuffer.push_back(image[((width*(y/ratio)+(x/ratio)))*4+2]);
                        textureArrayBuffer.push_back(image[((width*(y/ratio)+(x/ratio)))*4+3]);
//                        textureArrayBuffer[((i*size*size)+size*y+x)*4] = image[((width*(y/ratio)+(x/ratio)))*4];
//                        textureArrayBuffer[((i*size*size)+size*y+x)*4+1] = image[((width*(y/ratio)+(x/ratio)))*4+1];
//                        textureArrayBuffer[((i*size*size)+size*y+x)*4+2] = image[((width*(y/ratio)+(x/ratio)))*4+2];
//                        textureArrayBuffer[((i*size*size)+size*y+x)*4+3] = image[((width*(y/ratio)+(x/ratio)))*4+3];
                    }
                }
                i++;
                continue;
            }
            for (int j = 0; j < size * size * 4; ++j) {
                textureArrayBuffer.push_back(image[j]);
//                textureArrayBuffer[(i*size*size*4)+j] = image[j];
            }
            i++;
        }
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
                 textureArrayBuffer.data());
//    delete[] textureArrayBuffer;
}

GLuint TextureAtlas::getTextureId() const {
    return textureId;
}

uint32_t TextureAtlas::getTextureCount() const {
    return textureCount;
}

uint32_t TextureAtlas::getTextureFromName(const char* name){
    return textureIndexes[std::string{name}];
}

uint8_t TextureAtlas::getFrameTime(uint32_t textureIdOriginal) {
    return frameTimes[textureIdOriginal];
}

std::vector<uint8_t> TextureAtlas::getFrames(uint32_t textureIdOriginal) {
    return frames[textureIdOriginal];
}

bool TextureAtlas::isAnimated(uint32_t textureIdOriginal) {
    return frameTimes.find(textureIdOriginal)!=frameTimes.end();
}

void TextureAtlas::tick(uint32_t tick) {
    for (uint32_t texture : animatedTextures){
        uint8_t nextFrame = (tick / frameTimes[texture]) % frames[texture].size() + 1;
        glCopyImageSubData(
                textureId,
                GL_TEXTURE_2D_ARRAY,
                0,
                0,
                0,
                texture+nextFrame,
                textureId,
                GL_TEXTURE_2D_ARRAY,
                0,
                0,
                0,
                texture,
                size,
                size,
                1
                );
    }
}
