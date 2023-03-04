#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

// Smart pointer declaration for GLFWwindow
struct DestroyGLFWWindowPtr
{
    void operator()(GLFWwindow* ptr)
    {
        glfwDestroyWindow(ptr);
    }
};
typedef std::unique_ptr<GLFWwindow, DestroyGLFWWindowPtr> window_t;

// bool checkForOpenGLErrors()
// {
//     bool isErrorFound = false;

//     int errorCode = glGetError();
//     while(errorCode != GL_NO_ERROR)
//     {
//         spdlog::error("glError: " + errorCode);
//         isErrorFound = true;
//         errorCode = glGetError();
//     }

//     return isErrorFound;
// }

// enum ShaderParameter
// {
//     ShaderType = GL_SHADER_TYPE,
//     DeleteStatus = GL_DELETE_STATUS,
//     CompileStatus = GL_COMPILE_STATUS,
//     InfoLogLength = GL_INFO_LOG_LENGTH,
//     ShaderSourceLength = GL_SHADER_SOURCE_LENGTH,
// };

// void requestShaderParameter(GLuint shaderId, ShaderParameter param, GLint* output)
// {
//     spdlog::info("Requesting parameters of shader " + shaderId);
//     glGetShaderiv(shaderId, param, output);
//     checkForOpenGLErrors();
// }

void initializeGLFW();

void initializeGLEW();

void printWelcomeMessage();

#endif // __UTILS_HPP__