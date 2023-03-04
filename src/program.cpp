#include "./program.hpp"

void requestProgramParameter(GLuint programId, ProgramParameter param, GLint* output)
{
    spdlog::info("Requesting parameters of program {}", programId);
    glGetProgramiv(programId, (GLenum) param, output);
    checkForOpenGLErrors();
}

void printProgramInfoLog(GLuint programId)
{
    int logLength;
    requestProgramParameter(programId, ProgramParameter::InfoLogLength, &logLength);
    if(logLength > 0)
    {
        auto log = std::unique_ptr<char[]>(new char[logLength]);
        glGetProgramInfoLog(programId, logLength, nullptr, log.get());
        spdlog::info("Program Info Log: {}", log.get());
    }
}

GLuint createRenderingProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
    GLuint id = glCreateProgram();

    glAttachShader(id, vertexShaderId);
    glAttachShader(id, fragmentShaderId);
    glLinkProgram(id);

    GLint wasLinked;
    requestProgramParameter(id, ProgramParameter::LinkStatus, &wasLinked);
    if(wasLinked ==  GL_FALSE)
    {
        spdlog::error("Linking of program {} failed", id);
        printProgramInfoLog(id);
    }
    else
        spdlog::info("Linking of program {} successful", id);

    return id;
}