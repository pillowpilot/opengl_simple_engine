#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <filesystem>
#include <fstream>

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

bool checkForOpenGLErrors();

void initializeGLFW();

void initializeGLEW();

void printWelcomeMessage();

// Compatibility wrapper around std::string
// Usage: glShaderSource(id, 1, StringHelper(sourceCode), NULL);
struct StringHelper {
  const char *p;
  StringHelper(const std::string& s) : p(s.c_str()) {}
  operator const char**() { return &p; }
};

std::string readFile(std::filesystem::path path);

#endif // __UTILS_HPP__