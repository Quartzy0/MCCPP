//
// Created by quartzy on 2/15/21.
//

#include <cstring>
#include "Mesh.h"

Mesh Mesh::generateBlock(GLfloat textureId, GLfloat x, GLfloat y, GLfloat z) {
    Vertex cube_vertices[] = {
            // front
            {0.0f + x, -0.0f + y,  1.0f + z, 0.0, 0.0, textureId},
            {1.0f + x, -0.0f + y,  1.0f + z, 1.0, 0.0, textureId},
            {1.0f + x,  1.0f + y,  1.0f + z, 1.0, 1.0, textureId},
            {-0.0f + x,  1.0f + y,  1.0f + z, 0.0, 1.0, textureId},
            // top
            {-0.0f + x,  1.0f + y,  1.0f + z, 0.0, 0.0, textureId},
            {1.0f + x,  1.0f + y,  1.0f + z, 1.0, 0.0, textureId},
            {1.0f + x,  1.0f + y, -0.0f + z, 1.0, 1.0, textureId},
            {-0.0f + x,  1.0f + y, -0.0f + z, 0.0, 1.0, textureId},
            // back
            {1.0f + x, -0.0f + y, -0.0f + z, 0.0, 0.0, textureId},
            {-0.0f + x, -0.0f + y, -0.0f + z, 1.0, 0.0, textureId},
            {-0.0f + x,  1.0f + y, -0.0f + z, 1.0, 1.0, textureId},
            {1.0f + x,  1.0f + y, -0.0f + z, 0.0, 1.0, textureId},
            // bottom
            {-0.0f + x, -0.0f + y, -0.0f + z, 0.0, 0.0, textureId},
            {1.0f + x, -0.0f + y, -0.0f + z, 1.0, 0.0, textureId},
            {1.0f + x, -0.0f + y,  1.0f + z, 1.0, 1.0, textureId},
            {-0.0f + x, -0.0f + y,  1.0f + z, 0.0, 1.0, textureId},
            // left
            {-0.0f + x, -0.0f + y, -0.0f + z, 0.0, 0.0, textureId},
            {-0.0f + x, -0.0f + y,  1.0f + z, 1.0, 0.0, textureId},
            {-0.0f + x,  1.0f + y,  1.0f + z, 1.0, 1.0, textureId},
            {-0.0f + x,  1.0f + y, -0.0f + z, 0.0, 1.0, textureId},
            // right
            {1.0f + x, -0.0f + y,  1.0f + z, 0.0, 0.0, textureId},
            {1.0f + x, -0.0f + y, -0.0f + z, 1.0, 0.0, textureId},
            {1.0f + x,  1.0f + y, -0.0f + z, 1.0, 1.0, textureId},
            {1.0f + x,  1.0f + y,  1.0f + z, 0.0, 1.0, textureId}

    };
    GLushort cube_elements[] = {
            // front
            0,  1,  2,
            2,  3,  0,
            // top
            4,  5,  6,
            6,  7,  4,
            // back
            8,  9, 10,
            10, 11,  8,
            // bottom
            12, 13, 14,
            14, 15, 12,
            // left
            16, 17, 18,
            18, 19, 16,
            // right
            20, 21, 22,
            22, 23, 20

    };

    std::vector<Vertex> v(cube_vertices, cube_vertices + sizeof cube_vertices / sizeof cube_vertices[0]);
    std::vector<GLushort> v1(cube_elements, cube_elements + sizeof cube_elements / sizeof cube_elements[0]);

    return Mesh(v1, v, 6*6, 4*6);
}

void Mesh::setTextureId(GLfloat textureId) {
    for (int i = 0;i<6*4;i++){
        vertices[i].textureId = textureId;
    }
}

Mesh Mesh::joinBlockMeshes(Mesh mesh1, Mesh mesh2) {
    std::vector<Vertex> newVertices;
    newVertices.reserve(mesh1.vertexCount + mesh2.vertexCount);
    newVertices.insert(newVertices.end(), mesh1.vertices.begin(), mesh1.vertices.end());
    newVertices.insert(newVertices.end(), mesh2.vertices.begin(), mesh2.vertices.end());

    std::vector<GLushort> newElements;
    newElements.reserve(mesh1.elementCount + mesh2.elementCount);
    newElements.insert(newElements.end(), mesh1.elements.begin(), mesh1.elements.end());
    for (GLushort val : mesh2.elements){
        newElements.push_back(val+mesh1.elementCount);
    }

    return Mesh(newElements, newVertices, mesh1.elementCount + mesh2.elementCount, mesh1.vertexCount + mesh2.vertexCount);
}

Mesh::Mesh(std::vector<GLushort> &elements, std::vector<Vertex> &vertices, size_t elementCount, size_t vertexCount) : elements(std::move(elements)),
    vertices(std::move(vertices)),
    elementCount(elementCount),
    vertexCount(vertexCount){

}
