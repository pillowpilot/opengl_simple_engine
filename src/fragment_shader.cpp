#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    return id;
}
