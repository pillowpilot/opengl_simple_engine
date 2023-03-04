#include "./shader.hpp"

bool Shader::getCompilationStatus() const 
{
    spdlog::info("Requesting parameters of shader {}", this->id);

    GLint output;
    glGetShaderiv(this->id, (GLenum) ShaderParameter::CompileStatus, &output);
    checkForOpenGLErrors();

    return output == GL_TRUE;
}

std::size_t Shader::getInfoLogLength() const
{
    GLint logLength;
    glGetShaderiv(this->id, (GLenum) ShaderParameter::InfoLogLength, &logLength);
    checkForOpenGLErrors();
    return (std::size_t) logLength;
}

std::string Shader::getInfoLog() const
{
    const auto logLength = getInfoLogLength();

    if(logLength > 0)
    {
        auto log = std::unique_ptr<char[]>(new char[logLength]);
        glGetShaderInfoLog(this->id, logLength, nullptr, log.get());

        const auto logString = std::string(log.get());

        spdlog::info("Shader Info Log: {}", logString);

        return logString;
    }
    else
        return std::string("");
}

void requestShaderParameter(GLuint shaderId, ShaderParameter param, GLint* output)
{
    spdlog::info("Requesting parameters of shader {}", shaderId);
    glGetShaderiv(shaderId, (GLenum) param, output);
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
