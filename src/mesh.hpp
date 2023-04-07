#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <vector>
#include <exception>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "./utils.hpp"
#include "spdlog/spdlog.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal; // Placeholder

    Vertex(float x, float y, float z){ position = glm::vec3(x, y, z); }
};

class Mesh 
{
    public:
        Mesh(std::vector<Vertex> const& vertices, std::vector<uint> const& indices); 

	Mesh(const Mesh& other) = delete; // copy constructor
	Mesh& operator=(const Mesh& other) = delete; // copy assignment
	
	Mesh(Mesh &&other) // move constructor
	: m_vaoId(other.m_vaoId), m_vboIds(other.m_vboIds), m_vertices(other.m_vertices),
	m_indices(other.m_indices)
	{ 
	    other.m_vaoId = 0;
	    std::fill(std::begin(other.m_vboIds), std::end(other.m_vboIds), 0);
	}
	
	Mesh& operator=(Mesh &&other) // move assignment
	{
	    if(this != &other)
	    {
		std::swap(m_vaoId, other.m_vaoId);
		std::swap(m_vboIds, other.m_vboIds);
		std::swap(m_vertices, other.m_vertices);
		std::swap(m_indices, other.m_indices);
	    }
	    return *this;
	}

	~Mesh()
	{
	    glDeleteBuffers(m_vboIds.size(), m_vboIds.data());
	    glDeleteBuffers(1, &m_vaoId);
	}

	void draw() const 
	{
	    glBindVertexArray(m_vaoId);
	    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	    glBindVertexArray(0);
	}
        auto getNumberOfVertices() const { return m_vertices.size(); }
        auto getNumberOfIndices() const { return m_indices.size(); }

    private:
        const size_t m_verticesVBOId = 0;
        const size_t m_indicesVBOId = 1;
	const size_t m_positionAttrIndex = 0; // In shader, see layout (location = 0) in vec3 position

        GLuint m_vaoId;
        std::vector<GLuint> m_vboIds;
        std::vector<Vertex> m_vertices;
        std::vector<uint> m_indices;
};

#endif // __MESH_HPP__

