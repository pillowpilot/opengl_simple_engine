#include "./fragment_shader.hpp"

GLuint createFragmentShader()
{
    const auto filepath = std::filesystem::path("../shaders/fragment_shader_all_blue.glsl");
    const auto sourceCode = readFile(filepath);
        
    GLuint id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(id, 1, StringHelper(sourceCode), NULL);
    glCompileShader(id);

    GLint wasCompiled;
    requestShaderParameter(id, ShaderParameter::CompileStatus, &wasCompiled);
    if(wasCompiled ==  GL_FALSE)
    {
        spdlog::error("Compilation of shader {} failed", id);
        printShaderInfoLog(id);
    }
    else
        spdlog::info("Compilation of shader {} successful", id);

    return id;
}
