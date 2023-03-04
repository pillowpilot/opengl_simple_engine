#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include "./utils.hpp"

enum ShaderParameter
{
    ShaderType = GL_SHADER_TYPE,
    DeleteStatus = GL_DELETE_STATUS,
    CompileStatus = GL_COMPILE_STATUS,
    InfoLogLength = GL_INFO_LOG_LENGTH,
    ShaderSourceLength = GL_SHADER_SOURCE_LENGTH,
};

void requestShaderParameter(GLuint shaderId, ShaderParameter param, GLint* output);

void printShaderInfoLog(GLuint shaderId);

#endif // __SHADER_HPP__