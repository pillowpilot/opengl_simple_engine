#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <vector>
#include <filesystem>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "./mesh.hpp"

class Model
{
    public:
        Model(std::filesystem::path const& filepath);
	void draw() const;

    private:
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Mesh> m_meshes;
};


#endif // __MODEL_HPP__

