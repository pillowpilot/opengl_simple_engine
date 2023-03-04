#include <iostream>
#include <exception>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./utils.cpp"
#include "./vertex_shader.hpp"
#include "./fragment_shader.hpp"
#include "spdlog/spdlog.h"

// Globals
#define numberOfVAOs 1
GLuint vao[numberOfVAOs];

GLuint createRenderingProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
    GLuint id = glCreateProgram();

    glAttachShader(id, vertexShaderId);
    glAttachShader(id, fragmentShaderId);
    glLinkProgram(id);

    return id;
}

void initialize()
{
    glGenVertexArrays(numberOfVAOs, vao);
    glBindVertexArray(vao[0]);
}

void updateWindow(window_t& window, GLuint programId, double currentTime)
{
    glUseProgram(programId);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

window_t createWindow(const int height, const int width, const std::string& title)
{
    window_t window(glfwCreateWindow(height, width, title.c_str(), nullptr, nullptr));
    glfwMakeContextCurrent(window.get());

    return window;
}

int main()
{
    const int windowHeight = 600;
    const int windowWidth = 600;
    const std::string windowTitle = "Simple Graphics Engine";

    printWelcomeMessage();
    initializeGLFW();
    window_t window = createWindow(windowHeight, windowWidth, windowTitle);

    initializeGLEW();
    
    glfwSwapInterval(1);

    GLuint vertexShaderId = createVertexShader();
    GLuint fragmentShaderId = createFragmentShader();
    GLuint programId = createRenderingProgram(vertexShaderId, fragmentShaderId);

    initialize();
    while(!glfwWindowShouldClose(window.get()))
    {
        updateWindow(window, programId, glfwGetTime());
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    // Destroy resources
    glfwTerminate();

    return EXIT_SUCCESS;
}