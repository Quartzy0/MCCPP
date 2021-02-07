//
// Created by quartzy on 2/6/21.
//

#ifndef MCCPP_VERTEXBUFFER_H
#define MCCPP_VERTEXBUFFER_H


#include <cstdint>

class VertexBuffer {
private:
    uint32_t id;
public:
    VertexBuffer();

    void bind();

    void unbind();

    void deleteBuffer();

    void uploadData(void* data, size_t size, GLenum usage);
};


#endif //MCCPP_VERTEXBUFFER_H
