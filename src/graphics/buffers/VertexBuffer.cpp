//
// Created by quartzy on 2/6/21.
//

#include <GL/glew.h>
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(){
    glGenBuffers(1, &id);
}

void VertexBuffer::uploadData(void *data, size_t size, GLenum usage) {
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::deleteBuffer() {
    glDeleteBuffers(1, &id);
}

void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
