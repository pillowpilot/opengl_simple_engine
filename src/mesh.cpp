#include "./mesh.hpp"

Mesh::Mesh(std::vector<Vertex> const& vertices, std::vector<uint> const& indices)
: m_vertices(vertices), m_indices(indices), m_vboIds{0, 0}
{
    // Generate Buffers
    glGenVertexArrays(1, &m_vaoId);

    glBindVertexArray(m_vaoId);
    glGenBuffers(m_vboIds.size(), m_vboIds.data());

    // Upload data
    glBindBuffer(GL_ARRAY_BUFFER, m_vboIds.at(m_verticesVBOId));
    glBufferData(GL_ARRAY_BUFFER, sizeof(decltype(m_vertices)::value_type) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds.at(m_indicesVBOId));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(m_indices)::value_type) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    // Specify data layout
    glEnableVertexAttribArray(m_positionAttrIndex);
    glVertexAttribPointer(m_positionAttrIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

    // TODO Add normals
    // glEnableVertexAttribArray(m_normalAttrIndex);
    // glVertexAttribPointer(m_normalAttrIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    spdlog::info("Mesh created");
}

