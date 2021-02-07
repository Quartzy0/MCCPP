//
// Created by quartzy on 2/6/21.
//

#ifndef MCCPP_SHADERPROGRAM_H
#define MCCPP_SHADERPROGRAM_H


#include <string>
#include <GL/glew.h>

class ShaderProgram {
private:
    uint32_t program = 0;
    int32_t attribute_coord2d;

    std::string sourceVertex, sourceFragment;
public:
    [[nodiscard]] uint32_t getProgram() const;

    [[nodiscard]] const std::string &getSourceVertex() const;

    [[nodiscard]] const std::string &getSourceFragment() const;

    [[nodiscard]] int32_t getAttributeCoord2D() const;

    ShaderProgram(std::string sourceVertex, std::string sourceFragment);

    bool init();

    void cleanup() const;
};


#endif //MCCPP_SHADERPROGRAM_H
