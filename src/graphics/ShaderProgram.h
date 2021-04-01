//
// Created by quartzy on 2/6/21.
//

#ifndef MCCPP_SHADERPROGRAM_H
#define MCCPP_SHADERPROGRAM_H


#include <string>
#include <vector>
#include <GL/glew.h>
#include <unordered_map>
#include <glm/glm.hpp>

class ShaderProgram {
private:
    uint32_t program = 0;

    std::string sourceVertex, sourceGeometry, sourceFragment;

    struct VertexAttribute{
    public:
        GLint id;
        GLint size;
        GLint type;
        GLint normalized;
        GLint stride;
        const void* offset;
    };

    std::vector<VertexAttribute> vertexAttributes;

    std::unordered_map<std::string, GLint> unifromCache;
    std::vector<const char*> compileTimeDefinitions;

    GLint getUniformLocation(const char* name);
public:

    [[nodiscard]] uint32_t getProgram() const;

    [[nodiscard]] const std::string &getSourceVertex() const;

    [[nodiscard]] const std::string &getSourceFragment() const;

    ShaderProgram(const char* sourceVertex, const char* sourceGeometry, const char* sourceFragment);

    bool init();

    void cleanup() const;

    void bind() const;

    void unbind() const;

    void addVertexAttrib(GLint id, GLint size, GLint type, GLint normalized, GLint stride, const void* offset);

    void specifyVertexAttributes();

    void addCompiletimeDefinition(const char* definition);

    void setUniform(const char* name, GLfloat val);

    void setUniform(const char* name, glm::mat4& val);

    //TEXTURE SLOT IS USED FOR SETTING TEXTURES NOT TEXTURE ID
    void setUniform(const char* name, GLint val);

    void setUniform(const char* name, GLsizei count, const GLint* value);

    void setUniformLightLevels(uint32_t *lightLevels, size_t size);
};

#endif //MCCPP_SHADERPROGRAM_H
