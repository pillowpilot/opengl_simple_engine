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

std::string readFile(std::filesystem::path path)
{
    spdlog::info("Reading file from {}", std::filesystem::absolute(path).string());

    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        file.open(path); // Throws exceptions if fails
        const auto fileSize = std::filesystem::file_size(path);
        auto buffer = std::string(fileSize, '\0');

        file.read(buffer.data(), fileSize);

        spdlog::info("File reading done");

        return buffer;
    }
    catch(std::system_error& e)
    {
        spdlog::error("Error while reading file {}", std::filesystem::absolute(path).string());
        spdlog::error("System error message: {}", e.code().message());

        return std::string("");
    }
}