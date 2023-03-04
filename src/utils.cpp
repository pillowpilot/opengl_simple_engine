#include "./utils.hpp"

bool checkForOpenGLErrors()
{
    bool isErrorFound = false;

    int errorCode = glGetError();
    while(errorCode != GL_NO_ERROR)
    {
        spdlog::error("glError: {}", errorCode);
        isErrorFound = true;
        errorCode = glGetError();
    }

    return isErrorFound;
}

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

    spdlog::info("GLFW version: {}", glfwVersion);
    spdlog::info("GLEW version: {}", glewVersion);
}
