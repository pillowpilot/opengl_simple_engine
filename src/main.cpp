#include <iostream>
#include <exception>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/transform.hpp>

#include "./utils.hpp"
#include "./program.hpp"
#include "./vertex_shader.hpp"
#include "./fragment_shader.hpp"
#include "spdlog/spdlog.h"

// Globals
#define numberOfVAOs 1
GLuint vao[numberOfVAOs];

float x = 0.0f;
float increment = 0.01f;

void initialize()
{
    glGenVertexArrays(numberOfVAOs, vao);
    glBindVertexArray(vao[0]);
}

const glm::mat4 scalingMatrix = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f)); // Scaling factors

void updateWindow(window_t& window, GLuint programId, double currentTime)
{
    // Clear background
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programId);

    auto scalingMatrixLocation = glGetUniformLocation(programId, "scale_matrix");
    glUniformMatrix4fv(scalingMatrixLocation, 1, GL_FALSE, glm::value_ptr(scalingMatrix));

    x += increment;
    if(x > 0.5f || x < -0.5f) // Consider x scaling factor also
        increment *= -1.0f;

    GLuint offsetLocation = glGetUniformLocation(programId, "offset");
    glProgramUniform1f(programId, offsetLocation, x);

    // glPointSize(30.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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

    const auto vertexShaderFilepath = std::filesystem::path("../shaders/vertex_shader_x_moving_triangle.glsl");
    const VertexShader vertexShader(vertexShaderFilepath);

    const auto fragmentShaderFilepath = std::filesystem::path("../shaders/fragment_shader_all_blue.glsl");
    const FragmentShader fragmentShader(fragmentShaderFilepath);
    
    GLuint programId = createRenderingProgram(vertexShader.getId(), fragmentShader.getId());

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