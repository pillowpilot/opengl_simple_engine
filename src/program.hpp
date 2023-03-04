#ifndef __PROGRAM_HPP__
#define __PROGRAM_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include "./utils.hpp"

enum class ProgramParameter
{
    DeleteStatus = GL_DELETE_STATUS,
    LinkStatus = GL_LINK_STATUS,
    ValidationStatus = GL_VALIDATE_STATUS,
    InfoLogLength = GL_INFO_LOG_LENGTH,
    NumberOfAttachedShaderObjects = GL_ATTACHED_SHADERS,
    NumberOfActiveAttributeVariables = GL_ACTIVE_ATTRIBUTES,
    NumberOfActiveUniformVariables = GL_ACTIVE_UNIFORMS,
    // TODO Add GL_ACTIVE_ATTRIBUTE_MAX_LENGTH and GL_ACTIVE_UNIFORM_MAX_LENGTH
};

void requestProgramParameter(GLuint programId, ProgramParameter param, GLint* output);

void printProgramInfoLog(GLuint programId);

#endif // __PROGRAM_HPP__