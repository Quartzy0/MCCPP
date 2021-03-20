//
// Created by quartzy on 2/15/21.
//

#ifndef MCCPP_MESH_H
#define MCCPP_MESH_H

#include <GL/glew.h>
#include <vector>
#include "../graphics/Vertex.h"
#include "../graphics/Texture.h"

class Mesh{
public:
    size_t elementCount;
    size_t vertexCount;
    std::vector<GLushort> elements;
    std::vector<Vertex> vertices;

    Mesh(std::vector<GLushort>& elements, std::vector<Vertex>& vertices, size_t elementCount, size_t vertexCount);

    void setTextureId(GLfloat textureId = 0);

    static Mesh generateBlock(GLfloat textureId = 0, GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);

    static Mesh joinBlockMeshes(Mesh mesh1, Mesh mesh2);
};

#endif //MCCPP_MESH_H
