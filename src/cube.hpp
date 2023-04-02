#ifndef __CUBE_HPP__
#define __CUBE_HPP__

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "./mesh.hpp"
#include "spdlog/spdlog.h"

namespace procedual
{

class Cube
{
    public:
        Cube(const float sideLength = 2.0f);
	void draw() const
	{
	    m_mesh.draw();
	}
    private:
	std::vector<Vertex> generateCubeVertices(const float sideLength) const;
	std::vector<uint> generateCubeIndices() const;
	Mesh m_mesh;
};

};

#endif // __CUBE_HPP__

