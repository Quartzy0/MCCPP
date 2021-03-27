//
// Created by quartzy on 2/6/21.
//

#include "ShaderProgram.h"

#include <sstream>
#include <fstream>
#include "../util/Log.h"

std::string readFile(const char* filePath){
    std::fstream fstream;
    fstream.open(filePath);
    std::ostringstream oss;
    oss << fstream.rdbuf();
    fstream.close();
    return oss.str();
}

ShaderProgram::ShaderProgram(const char* sourceVertex, const char* sourceGeometry, const char* sourceFragment) {
    this->sourceVertex = readFile(sourceVertex);
    this->sourceGeometry = readFile(sourceGeometry);
    this->sourceFragment = readFile(sourceFragment);
}


bool ShaderProgram::init(uint32_t pointLightCount) {
    this->pointLightCount = pointLightCount;
    std::string str = "#define POINT_LIGHT_COUNT " + std::to_string(pointLightCount);
    compileTimeDefinitions.push_back(str.c_str());
    for (const char* def : compileTimeDefinitions){
        sourceVertex.insert(18, def);
        sourceGeometry.insert(63, def);
        sourceFragment.insert(18, def);
    }

    if (!GLEW_EXT_geometry_shader4){
        CORE_ERROR("Geometry shaders not supported. Aborting");
        exit(1);
    }

    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

    //Vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char *vs_source = sourceVertex.c_str();

    glShaderSource(vs, 1, &vs_source, nullptr);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        GLint maxLength = 0;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vs, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(vs); // Don't leak the shader.
        CORE_ERROR("Error while compiling vertex shader: {}", errorLog.data());
        return false;
    }

    //Geometry shader
    GLuint gs = glCreateShader(GL_GEOMETRY_SHADER_EXT);
    const char *gs_source = sourceGeometry.c_str();

    glShaderSource(gs, 1, &gs_source, nullptr);
    glCompileShader(gs);
    glGetShaderiv(gs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        GLint maxLength = 0;
        glGetShaderiv(gs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(gs, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(gs); // Don't leak the shader.
        CORE_ERROR("Error while compiling geometry shader: {}", errorLog.data());
        return false;
    }

    //Fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fs_source = sourceFragment.c_str();

    glShaderSource(fs, 1, &fs_source, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        GLint maxLength = 0;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fs, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(fs); // Don't leak the shader.
        CORE_ERROR("Error while compiling fragment shader: {}", errorLog.data());
        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, gs);
    glAttachShader(program, fs);

    //Geometry shader parameters
    glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, GL_LINES);
    glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

        glDeleteProgram(program);
        CORE_ERROR("Error in glLinkProgram: {}", errorLog.data());
        return false;
    }

    specifyVertexAttributes();

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

void ShaderProgram::cleanup() const {
    glDeleteProgram(program);
}

void ShaderProgram::unbind() const{
    glUseProgram(0);
}

void ShaderProgram::bind() const {
    glUseProgram(program);
}

void ShaderProgram::addVertexAttrib(GLint id, GLint size, GLint type, GLint normalized, GLint stride, const void* offset) {
    vertexAttributes.push_back({id, size, type, normalized, stride, offset});
}

void ShaderProgram::specifyVertexAttributes() {
    for (const VertexAttribute& va : vertexAttributes) {
        glEnableVertexAttribArray(va.id);
        glVertexAttribPointer(va.id, va.size, va.type, va.normalized, va.stride, va.offset);
    }
}

GLint ShaderProgram::getUniformLocation(const char* name) {
    auto location = unifromCache.find(name);

    if (location == unifromCache.end())
    {
        GLint uniformLocation = glGetUniformLocation(program, name);
        unifromCache.emplace(name, uniformLocation);
        return uniformLocation;
    }
    return location->second;
}

void ShaderProgram::setUniform(const char* name, GLfloat val) {
    GLint uniformLocation = getUniformLocation(name);
    glUniform1f(uniformLocation, val);
}

void ShaderProgram::setUniform(const char *name, glm::mat4& val) {
    GLint uniformLocation = getUniformLocation(name);
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &val[0][0]);
}

//TEXTURE SLOT IS USED FOR SETTING TEXTURES NOT TEXTURE ID
void ShaderProgram::setUniform(const char *name, GLint val) {
    GLint uniformLocation = getUniformLocation(name);
    glUniform1i(uniformLocation, val);
}

void ShaderProgram::setUniform(const char* name, GLsizei count, const GLint* value) {
    GLint uniformLocation = getUniformLocation(name);
    glUniform1iv(uniformLocation, count, value);
}

void ShaderProgram::addCompiletimeDefinition(const char *definition) {
    compileTimeDefinitions.push_back(definition);
}

void ShaderProgram::setPointLights(PointLight *pointLight, size_t count) {
    if (count>pointLightCount){
        CORE_WARN("More point lights were provided than could be used");
    }
    static char buffer[64];
    for (int i = 0; i < pointLightCount; ++i) {
        sprintf(buffer, "pointLights[%u].vColor",i);
        GLuint location_color = getUniformLocation(buffer);
        sprintf(buffer, "pointLights[%u].vPosition",i);
        GLuint location_position = getUniformLocation(buffer);
        sprintf(buffer, "pointLights[%u].fAmbient",i);
        GLuint location_ambient = getUniformLocation(buffer);
        sprintf(buffer, "pointLights[%u].fConstantAtt",i);
        GLuint location_constantAtt = getUniformLocation(buffer);
        sprintf(buffer, "pointLights[%u].fLinearAtt",i);
        GLuint location_linearAtt = getUniformLocation(buffer);
        sprintf(buffer, "pointLights[%u].fExpAtt",i);
        GLuint location_quadAtt = getUniformLocation(buffer);
        if (i>=count){
            glUniform3f(location_color, 0, 0, 0);
            glUniform3f(location_position, 0, 0, 0);

            glUniform1f(location_ambient, 0);

            glUniform1f(location_constantAtt, 1);
            glUniform1f(location_linearAtt, 0);
            glUniform1f(location_quadAtt, 0);
        }else{
            glUniform3f(location_color, pointLight[i].color.r, pointLight[i].color.g, pointLight[i].color.b);
            glUniform3f(location_position, pointLight[i].position.x, pointLight[i].position.y, pointLight[i].position.z);

            glUniform1f(location_ambient, pointLight[i].ambient);

            glUniform1f(location_constantAtt, pointLight[i].constantAtt);
            glUniform1f(location_linearAtt, pointLight[i].linearAtt);
            glUniform1f(location_quadAtt, pointLight[i].quadAtt);
        }
    }
}
