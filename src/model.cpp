#include "./model.hpp"

void Model::processNode(aiNode *nodePtr, const aiScene *scenePtr)
{
    // Add node's associated mesh into the model
    for(size_t i = 0; i < nodePtr->mNumMeshes; ++i)
    {
	const auto meshId = nodePtr->mMeshes[i];
	const auto meshPtr = scenePtr->mMeshes[meshId];

	auto mesh = processMesh(meshPtr, scenePtr);
	m_meshes.push_back(std::move(mesh));
    }

    // Recursively process each child
    for(size_t i = 0; i < nodePtr->mNumChildren; ++i)
    {
	const auto childPtr = nodePtr->mChildren[i];
	processNode(childPtr, scenePtr);
    }
}

Mesh Model::processMesh(aiMesh *meshPtr, const aiScene *scenePtr)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(meshPtr->mNumVertices);
    for(size_t i = 0; i < meshPtr->mNumVertices; ++i)
    {
        Vertex vertex(meshPtr->mVertices[i].x, meshPtr->mVertices[i].y, meshPtr->mVertices[i].z);
        vertices.push_back(vertex);
    }

    indices.reserve(meshPtr->mNumFaces); // aiProcess_Triangulate implies that every face is a triangle
    for(size_t i = 0; i < meshPtr->mNumFaces; ++i)
    {
        const aiFace face = meshPtr->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    Mesh mesh(vertices, indices);
    return mesh;
}

Model::Model(std::filesystem::path const& filepath)
{
   Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath.string(),
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        spdlog::error("Failed to import model from {}", filepath.string());
        spdlog::error("Details: {}", importer.GetErrorString());
    }
    spdlog::info("Model imported from {}", filepath.string());

    const auto root = scene->mRootNode;
    processNode(root, scene);
}

void Model::draw() const
{
    for(const auto& m: m_meshes)
	m.draw();
}

