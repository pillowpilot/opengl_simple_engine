#ifndef __PROGRAM_HPP__
#define __PROGRAM_HPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "spdlog/spdlog.h"

#include "./vertex_shader.hpp"
#include "./fragment_shader.hpp"
#include "./utils.hpp"

enum class ProgramParameter
{
    DeleteStatus = GL_DELETE_STATUS,
    LinkStatus = GL_LINK_STATUS,
    ValidationStatus = GL_VALIDATE_STATUS,
    InfoLogLength = GL_INFO_LOG_LENGTH,
    NumberOfAttachedShaderObjects = GL_ATTACHED_SHADERS,
    NumberOfActiveAttributeVariables = GL_ACTIVE_ATTRIBUTES,
    NumberOfActiveUniformVariables = GL_ACTIVE_UNIFORMS,
    // TODO Add GL_ACTIVE_ATTRIBUTE_MAX_LENGTH and GL_ACTIVE_UNIFORM_MAX_LENGTH
};

void requestProgramParameter(GLuint programId, ProgramParameter param, GLint* output);

void printProgramInfoLog(GLuint programId);

GLuint createRenderingProgram(GLuint vertexShaderId, GLuint fragmentShaderId);

class RenderingProgram
{
public:
    RenderingProgram(const VertexShader& vs, const FragmentShader& fs)
    : m_vertexShader(vs), m_fragmentShader(fs), 
	id(createRenderingProgram(vs.getId(), fs.getId()))
    {}
    void use() const
    {
	glUseProgram(id);
    }
    void setUniform(const std::string& name, float value) const
    {
	const auto location = glGetUniformLocation(id, name.c_str());
	glUniform1f(location, value);
    }
    void setUniform(const std::string& name, const glm::vec3& value) const
    {
	const auto location = glGetUniformLocation(id, name.c_str());
	glUniform3fv(location, 1, glm::value_ptr(value));
    }
    void setUniform(const std::string& name, const glm::mat4& value) const
    {
	const auto location = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    auto getId() const { return id; }

private:
    const VertexShader m_vertexShader;
    const FragmentShader m_fragmentShader;
    const GLuint id;
};

#endif // __PROGRAM_HPP__
