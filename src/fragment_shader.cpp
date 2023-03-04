#include "./fragment_shader.hpp"

GLuint createFragmentShader()
{
    const char* sourceCode = 
        "#version 430 \n"
        "out vec4 color; \n"
        "void main(void) \n"
        "{ color = vec4(0.0, 0.0, 1.0, 1.0); }";

    GLuint id = glCreateShader(GL_FRAGMENT_SHADER);
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
