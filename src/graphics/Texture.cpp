//
// Created by quartzy on 2/15/21.
//

#include <vector>
#include "Texture.h"
#include "../util/Log.h"
#include "../lodepng/lodepng.h"

void Texture::loadTextureFromFile(std::string &filename) {
    std::vector<unsigned char> image;
    unsigned width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, filename);

    //if there's an error, display it
    if (error){
        CORE_ERROR("Error loading texture");
        return;
    }
    w = width;
    h = height;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, // target
                 0, // level, 0 = base, no minimap,
                 GL_RGBA, // internalformat
                 w, // width
                 h, // height
                 0, // border, always 0 in OpenGL ES
                 GL_RGBA, // format
                 GL_UNSIGNED_BYTE, // type
                 &image[0]);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(GLuint slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::cleanup() {
    glDeleteTextures(0, &textureId);
}

GLuint Texture::getTextureId() const {
    return textureId;
}

GLuint Texture::getW() const {
    return w;
}

GLuint Texture::getH() const {
    return h;
}
