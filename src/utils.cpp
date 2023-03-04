#pragma once

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

void initializeGLFW()
{
    if(!glfwInit())
        throw new std::runtime_error("Could not initialize GLFW");

    // Request OpenGL 4.3+
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // TODO Add error management if this fails (if 4.3 is not supported, it fails)

    spdlog::info("GLFW initialized correctly.");
    spdlog::info("Working with OpenGL 4.3+.");
}

void initializeGLEW()
{
    if(glewInit() != GLEW_OK)
        throw new std::runtime_error("Could not initialize GLEW");

    spdlog::info("GLEW initialized correctly.");
}

void printWelcomeMessage()
{
    const auto glfwVersion = std::string(glfwGetVersionString());
    const auto glewVersion = std::string(reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)));

    spdlog::info("GLFW version: " + glfwVersion);
    spdlog::info("GLEW version: " + glewVersion);
}
