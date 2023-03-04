#include "./vertex_shader.hpp"

GLuint createVertexShader()
{
    const char* sourceCode = 
        "#version 420 \n"
        "void main(void) \n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    GLuint id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id, 1, &sourceCode, NULL);
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
