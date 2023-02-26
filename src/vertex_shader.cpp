#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint createVertexShader()
{
    const char* sourceCode = 
        "#version 420 \n"
        "void main(void) \n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    GLuint id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id, 1, &sourceCode, NULL);
    glCompileShader(id);

    return id;
}
