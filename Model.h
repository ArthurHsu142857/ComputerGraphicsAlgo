#pragma once

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <unordered_map>


class Model {
public:
	Model();
	Model(const char* path);

	void Draw(GLuint programID);

private:
	void LoadModel(const std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

	std::string mDirectory;
	std::vector<Mesh> mMeshes;
	std::unordered_map<std::string, int> textureMap;
};

