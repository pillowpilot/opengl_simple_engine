#include "./vertex_shader.hpp"

GLuint createVertexShader()
{
    const auto filepath = std::filesystem::path("../shaders/vertex_shader_origin_point.glsl");
    const auto sourceCode = readFile(filepath);

    GLuint id = glCreateShader(GL_VERTEX_SHADER);
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
