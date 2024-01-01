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
	enum ModelType {
		LOAD_MODEL,
		FULL_SCREEN_QUAD
	};

public:
	Model(ModelType type, const char* path = nullptr);

	void Draw(Shader* shader);

private:
	static constexpr float smQuadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	static constexpr unsigned int smQuadIndices[] = {
		0, 1, 2,
		3, 4, 5
	};

private:
	void BuildModel(const std::string path);
	void BuildQuad();
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	Color LoadMaterialColors(aiMaterial* mat);
	Mesh ProcessQuadMesh();

	std::string mDirectory;
	std::vector<Mesh> mMeshes;
	std::unordered_map<std::string, int> textureMap;
};

