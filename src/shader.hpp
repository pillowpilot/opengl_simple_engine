#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include "./utils.hpp"

class Shader
{
protected:
    std::filesystem::path filepath;
    GLuint id;

public:
    auto getId() const { return id; }
    bool getCompilationStatus() const;
    std::size_t getInfoLogLength() const;
    std::string getInfoLog() const;
};

enum class ShaderParameter
{
    ShaderType = GL_SHADER_TYPE,
    DeleteStatus = GL_DELETE_STATUS,
    CompileStatus = GL_COMPILE_STATUS,
    InfoLogLength = GL_INFO_LOG_LENGTH,
    ShaderSourceLength = GL_SHADER_SOURCE_LENGTH,
};

[[deprecated]]
void requestShaderParameter(GLuint shaderId, ShaderParameter param, GLint* output);

[[deprecated]]
void printShaderInfoLog(GLuint shaderId);

#endif // __SHADER_HPP__