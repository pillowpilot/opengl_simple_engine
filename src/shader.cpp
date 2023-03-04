#include "./shader.hpp"

void requestShaderParameter(GLuint shaderId, ShaderParameter param, GLint* output)
{
    spdlog::info("Requesting parameters of shader {}", shaderId);
    glGetShaderiv(shaderId, param, output);
    checkForOpenGLErrors();
}

void printShaderInfoLog(GLuint shaderId)
{
    int logLength;
    requestShaderParameter(shaderId, ShaderParameter::InfoLogLength, &logLength);
    if(logLength > 0)
    {
        auto log = std::unique_ptr<char[]>(new char[logLength]);
        glGetShaderInfoLog(shaderId, logLength, nullptr, log.get());
        spdlog::info("Shader Info Log: {}", log.get());
    }
}
