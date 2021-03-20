//
// Created by quartzy on 2/15/21.
//

#ifndef MCCPP_TEXTURE_H
#define MCCPP_TEXTURE_H

#include <GL/glew.h>
#include <string>


class Texture {
private:
    GLuint textureId;

    GLuint w;
    GLuint h;
public:
    Texture() = default;

    void loadTextureFromFile(std::string& filename);

    void bind(GLuint slot = 0);

    void unbind();

    void cleanup();

    GLuint getTextureId() const;

    GLuint getW() const;

    GLuint getH() const;
};


#endif //MCCPP_TEXTURE_H
