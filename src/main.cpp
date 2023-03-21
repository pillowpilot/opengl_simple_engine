#include <iostream>
#include <exception>
#include <memory>
#include <array>
#include <vector>

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
#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>

// Globals
const size_t numberOfVAOs = 1;
std::array<GLuint, numberOfVAOs> VAOIds;

const size_t numberOfVBOs = 2;
std::array<GLuint, numberOfVBOs> VBOIds;

auto cameraOffset = glm::vec3(0.0f, 0.0f, 16.0f);
glm::mat4 perspectiveMatrix;

void loadGeometry(const GLuint vboId)
{
    std::vector<float> vertices;
    {
        const auto filepath = std::filesystem::path("../figures/pyramid.txt");
        std::ifstream inputFile(filepath, std::ios_base::in);
        cereal::JSONInputArchive ar(inputFile);

        ar(vertices);
        spdlog::info("Loaded vertices from {}", std::filesystem::absolute(filepath).string());
    }

    const auto sizeOfBufferInBytes = sizeof(float) * vertices.size();

    glBindBuffer(GL_ARRAY_BUFFER, vboId); // GL_ARRAY_BUFFER is for Vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeOfBufferInBytes, vertices.data(), GL_STATIC_DRAW);
}

void windowReshapeCallback(GLFWwindow* window, int newWidth, int newHeight)
{
    const float aspectRatio = ((float) newWidth) / newHeight;
    perspectiveMatrix = glm::perspective(1.04f, aspectRatio, 0.1f, 1000.0f); // 60 degrees = 1.04f radians

    glViewport(0, 0, newWidth, newHeight);
}

void initialize(window_t& window)
{
    // Initialize perspective matrix
    int windowHeight, windowWidth;
    glfwGetFramebufferSize(window.get(), &windowWidth, &windowHeight);
    const float aspectRatio = ((float) windowWidth) / windowHeight;
    perspectiveMatrix = glm::perspective(1.04f, aspectRatio, 0.1f, 1000.0f); // 60 degrees = 1.04f radians

    // Set window resize callback
    glfwSetWindowSizeCallback(window.get(), windowReshapeCallback);

    // Initialize geometry DS
    glGenVertexArrays(sizeof(VAOIds), VAOIds.data());
    glBindVertexArray(VAOIds.at(0));
    glGenBuffers(sizeof(VBOIds), VBOIds.data());

    loadGeometry(VBOIds.at(0)); // Cube's positions are in vbo[0]
}

void updateWindow(window_t& window, GLuint programId, double currentTime)
{
    // Clear background
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programId);

    // Upload perspective matrix
    const auto perspectiveMatrixLocation = glGetUniformLocation(programId, "proj_matrix");
    glUniformMatrix4fv(perspectiveMatrixLocation, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));

    // Upload time factor
    const auto timeFactorLocation = glGetUniformLocation(programId, "time_factor");
    glUniform1f(timeFactorLocation, currentTime);

    // Associate VBO with the corresponding vertex attribute in the vertex shader
    glBindBuffer(GL_ARRAY_BUFFER, VBOIds.at(0)); // Cube's positions are in vbo[0]
    const auto indexOfVertexAttribute = 0; // See layout (location = 0) in vec3 position
    const auto numberOfComponentsPerVAD = 3;
    const auto typeOfEachComponent = GL_FLOAT;
    const auto shouldNormalizeData = GL_FALSE;
    const auto strideBetweenVAs = 0;
    const auto initialOffset = reinterpret_cast<void*>(0);
    glVertexAttribPointer(indexOfVertexAttribute, numberOfComponentsPerVAD, typeOfEachComponent, shouldNormalizeData, strideBetweenVAs, initialOffset);
    glEnableVertexAttribArray(indexOfVertexAttribute);

    // Adjust OpenGL settings and draw
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // Cube has 36 vertices.
    // Pyramid, 18.
    glDrawArraysInstanced(GL_TRIANGLES, 0, 18, 24);
    
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

    const auto vertexShaderFilepath = std::filesystem::path("../shaders/vertex_shader_dynamic_cube.glsl");
    const VertexShader vertexShader(vertexShaderFilepath);

    const auto fragmentShaderFilepath = std::filesystem::path("../shaders/fragment_shader_dynamic_cube.glsl");
    const FragmentShader fragmentShader(fragmentShaderFilepath);
    
    GLuint programId = createRenderingProgram(vertexShader.getId(), fragmentShader.getId());

    initialize(window);
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