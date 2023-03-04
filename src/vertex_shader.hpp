#ifndef __VERTEX_SHADER_HPP__
#define __VERTEX_SHADER_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./utils.hpp"
#include "./shader.hpp"

class VertexShader: public Shader
{
public:
    VertexShader(const std::filesystem::path& path)
    {
        const auto sourceCode = readFile(path);

        this->id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(this->id, 1, StringHelper(sourceCode), NULL);
        glCompileShader(this->id);

        const auto wasCompiled = this->getCompilationStatus();
        if(!wasCompiled)
        {
            spdlog::error("Compilation of shader {} failed", this->id);
            const auto log = this->getInfoLog();
            spdlog::info(log);
        }
        else
            spdlog::info("Compilation of shader {} successful", this->id);
    }
};

[[deprecated]]
GLuint createVertexShader();

#endif // __VERTEX_SHADER_HPP__