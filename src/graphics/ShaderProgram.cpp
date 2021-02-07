//
// Created by quartzy on 2/6/21.
//

#include "ShaderProgram.h"

#include <utility>
#include <iostream>

ShaderProgram::ShaderProgram(std::string sourceVertex, std::string sourceFragment) : sourceVertex(std::move(
        sourceVertex)), sourceFragment(std::move(sourceFragment)) {}

bool ShaderProgram::init() {
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char *vs_source =
            //"#version 100\n"  // OpenGL ES 2.0
            "#version 150 core\n"  // OpenGL 2.1
            "in vec2 coord2d;                  "
            "void main(void) {                        "
            "  gl_Position = vec4(coord2d, 0.0, 1.0); "
            "}";
    glShaderSource(vs, 1, &vs_source, nullptr);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        std::cerr << "Error in vertex shader\n";
        return false;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs_source =
            //"#version 100\n"  // OpenGL ES 2.0
            "#version 150 core\n"  // OpenGL 2.1
            "void main(void) {        "
            "  gl_FragColor[0] = 1.0; "
            "  gl_FragColor[1] = 1.0; "
            "  gl_FragColor[2] = 1.0; "
            "}";
    glShaderSource(fs, 1, &fs_source, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        std::cerr << "Error in fragment shader\n";
        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        std::cerr << "Error in glLinkProgram\n";
        return false;
    }

    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord2d == -1) {
        std::cerr << "Could not bind attribute " << attribute_name << "\n";
        return false;
    }

    return true;
}

uint32_t ShaderProgram::getProgram() const {
    return program;
}

const std::string &ShaderProgram::getSourceVertex() const {
    return sourceVertex;
}

const std::string &ShaderProgram::getSourceFragment() const {
    return sourceFragment;
}

int32_t ShaderProgram::getAttributeCoord2D() const {
    return attribute_coord2d;
}

void ShaderProgram::cleanup() const {
    glDeleteProgram(program);
}
