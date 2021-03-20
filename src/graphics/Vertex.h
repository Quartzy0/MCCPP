//
// Created by quartzy on 2/14/21.
//

#ifndef MCCPP_VERTEX_H
#define MCCPP_VERTEX_H

struct Vertex{
    GLbyte x = 0, y = 0, z = 0;

    GLshort textureId = 0;

    Vertex(GLbyte x1, GLbyte y1, GLbyte z1, GLshort textureId1) : x(x1), y(y1), z(z1), textureId(textureId1) {}

    Vertex() = default;

    ~Vertex() = default;
};

#endif //MCCPP_VERTEX_H
