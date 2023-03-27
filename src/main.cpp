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

void loadGeometry(const GLuint vboId, const GLuint indexesId)
{
    std::vector<float> vertices;
    {
        const auto filepath = std::filesystem::path("../figures/pyramid.txt");
        std::ifstream inputFile(filepath, std::ios_base::in);
        cereal::JSONInputArchive ar(inputFile);

        ar(vertices);
        spdlog::info("Loaded vertices from {}", std::filesystem::absolute(filepath).string());
    }

    std::vector<glm::vec3> vertices_v2 = {
        glm::vec3(+1, 0, +1), // 0
        glm::vec3(-1, 0, +1), // 1
        glm::vec3(+1, 0, -1), // 2
        glm::vec3(-1, 0, -1), // 3
        glm::vec3( 0, 1,  0), // 4
    };

    std::vector<unsigned int> indices_v2 = {
        0, 1, 2, // base 1
        2, 3, 1, // base 2
        2, 4, 0, // side 1
        3, 4, 2, // side 2
        1, 4, 3, // side 3
        1, 4, 0, // side 4
    };

    glBindBuffer(GL_ARRAY_BUFFER, vboId); // GL_ARRAY_BUFFER is for Vertex attributes
    glBufferData(GL_ARRAY_BUFFER, sizeof(decltype(vertices_v2)::value_type) * vertices_v2.size(), vertices_v2.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices_v2)::value_type) * indices_v2.size(), indices_v2.data(), GL_STATIC_DRAW);
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

    loadGeometry(VBOIds.at(0), VBOIds.at(1)); // Cube's positions are in vbo[0]
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
    // glDrawArraysInstanced(GL_TRIANGLES, 0, 18, 24);
    glBindBuffer(GL_ARRAY_BUFFER, VBOIds.at(0));
    glDrawElementsInstanced(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0, 24);
    
}

void glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

window_t createWindow(const int height, const int width, const std::string& title)
{
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); 
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

    int flags; 
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        spdlog::info("Initializing debug output");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE); 
    }
    
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