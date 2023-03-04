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
