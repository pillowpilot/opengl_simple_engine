#include <iostream>
#include <exception>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "./utils.hpp"
#include "./program.hpp"
#include "./vertex_shader.hpp"
#include "./fragment_shader.hpp"
#include "spdlog/spdlog.h"
// Serialization library
#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>

#include "./model.hpp"
#include "./cube.hpp"

// Globals

auto cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 perspectiveMatrix;

// Do not instanciate a class that depends on OpenGL (as Mesh, Cube, etc.) because OpenGL is *not* initialized yet!
std::shared_ptr<Model> model;
std::shared_ptr<procedural::Cube> cube;
std::shared_ptr<procedural::Cube> lightA;

void loadGeometry()
{
    const auto filepath = std::filesystem::path("../figures/teapot.obj");
    model = std::make_shared<Model>(filepath);
    cube = std::make_shared<procedural::Cube>();
    lightA = std::make_shared<procedural::Cube>();
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

    loadGeometry(); 
}

void processInput(window_t& window)
{
    // Set window to close
    if(glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
	glfwSetWindowShouldClose(window.get(), true);
	spdlog::info("Closing...");
    }

    // Update camera position
    const auto cameraSpeed = 0.15f; // TODO Magic number
    if(glfwGetKey(window.get(), GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += cameraSpeed * cameraFront;	
    }
    else if(glfwGetKey(window.get(), GLFW_KEY_S) == GLFW_PRESS)
    {
	cameraPosition -= cameraSpeed * cameraFront;
    }
    else if(glfwGetKey(window.get(), GLFW_KEY_A) == GLFW_PRESS)
    {
	cameraPosition -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    else if(glfwGetKey(window.get(), GLFW_KEY_D) == GLFW_PRESS)
    {
	cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    } 
}

void updateWindow(window_t& window, RenderingProgram program, RenderingProgram lightingProgram, double currentTime)
{
    // Process keyboard/mouse input
    processInput(window);

    // Clear background
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    // View matrix
    const auto view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

    const auto objectColor = glm::vec3(1.0, 0.5, 0.3);
    const auto lightColor = glm::vec3(1.0, 1.0, 1.0);

    // Lighting
    lightingProgram.use();
    lightingProgram.setUniform("viewMatrix", view);
    lightingProgram.setUniform("projectionMatrix", perspectiveMatrix);
    lightingProgram.setUniform("objectColor", objectColor);
    lightingProgram.setUniform("lightColor", lightColor);
    

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    lightA->draw();
    cube->draw();

    program.use();
    program.setUniform("proj_matrix", perspectiveMatrix);
    program.setUniform("time_factor", currentTime);

    // Adjust OpenGL settings and draw
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Draw wireframe
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    // glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); // Disable wireframe

    model->draw();
    // cube->draw();
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

    const VertexShader vertexShader(
		    std::filesystem::path("../shaders/vertex_shader_dynamic_cube.glsl")); 
    const FragmentShader fragmentShader(
		    std::filesystem::path("../shaders/fragment_shader_dynamic_cube.glsl")); 
    const RenderingProgram program(vertexShader, fragmentShader);

    const VertexShader lightingVertexShader(
		    std::filesystem::path("../shaders/vertex_shader_light.glsl"));
    const FragmentShader lightingFragmentShader(
		    std::filesystem::path("../shaders/fragment_shader_white.glsl"));
    const RenderingProgram lightingProgram(lightingVertexShader, lightingFragmentShader);

    initialize(window);
    while(!glfwWindowShouldClose(window.get()))
    {
        updateWindow(window, program, lightingProgram, glfwGetTime());
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    // Destroy resources
    glfwTerminate();

    return EXIT_SUCCESS;
}
