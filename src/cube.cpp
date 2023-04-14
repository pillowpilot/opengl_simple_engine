#include "./cube.hpp"

namespace procedual
{

Cube::Cube(const float sideLength)
:m_mesh(generateCubeVertices(sideLength), generateCubeIndices())
{}

std::vector<Vertex> Cube::generateCubeVertices(const float sideLength) const
{
    const float halfSide = sideLength / 2.0f;
    std::vector<Vertex> vertices;

    vertices.push_back(Vertex(+halfSide, +halfSide, +halfSide));
    vertices.push_back(Vertex(+halfSide, +halfSide, -halfSide));
    vertices.push_back(Vertex(+halfSide, -halfSide, +halfSide));
    vertices.push_back(Vertex(+halfSide, -halfSide, -halfSide));
    vertices.push_back(Vertex(-halfSide, +halfSide, +halfSide));
    vertices.push_back(Vertex(-halfSide, +halfSide, -halfSide));
    vertices.push_back(Vertex(-halfSide, -halfSide, +halfSide));
    vertices.push_back(Vertex(-halfSide, -halfSide, -halfSide));

    return vertices;
}

std::vector<unsigned int> Cube::generateCubeIndices() const
{
    std::vector<unsigned int> indices{
	0, 1, 2,
	1, 3, 2,
	4, 5, 6,
	5, 7, 6,
	4, 5, 0, 
	0, 5, 1,
	7, 5, 1,
	1, 3, 7,
	2, 6, 7, 
	2, 7, 3, 
	2, 6, 4,
	2, 4, 0,
    };

    return indices;
}

}

