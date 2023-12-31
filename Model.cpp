#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

Model::Model() {

}

Model::Model(const char* path) {
	LoadModel(path);
}

void Model::Draw(Shader* shader) {
    for (int i = 0; i < mMeshes.size(); i++) {
        mMeshes[i].Draw(shader);
    }
}

void Model::LoadModel(const std::string path) {
    std::cout << "Load model from path : " << path << std::endl << std::endl;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    mDirectory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.emplace_back(ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    std::cout << "Get " << mesh->mName.C_Str() << " mesh with " << mesh->mNumVertices << " vertices" << std::endl;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        if (mesh->HasTextureCoords(0)) {
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        if (mesh->HasTangentsAndBitangents()) {
            vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
            vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        }
        else {
            vertex.texCoords = glm::vec3(0.0f, 0.0f, 0.0f);
            vertex.texCoords = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        vertices.emplace_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    Color color = LoadMaterialColors(material);

    std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::cout << "\tDiffuse maps : " << diffuseMaps.size() << std::endl;
    
    std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::cout << "\tSpecular maps : " << specularMaps.size() << std::endl;

    std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::cout << "\tNormal maps : " << normalMaps.size() << std::endl;

    std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    std::cout << "\tHeight maps : " << heightMaps.size() << std::endl;

    std::cout << "Done mesh process" << std::endl << std::endl;

    return Mesh(vertices, color, indices, textures);
}



vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);

        std::string texturePath = str.C_Str();
        auto it = textureMap.find(texturePath);
        if (it == textureMap.end()) {
            textureMap.insert(std::pair<std::string, int>(texturePath, i));

            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->mDirectory);
            texture.type = typeName;
            texture.path = str.C_Str();
            
            textures.emplace_back(texture);
        }
    }
    
    return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string &directory, bool gamma) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

Color Model::LoadMaterialColors(aiMaterial* mat) {
    Color returnColor;
    aiString matName;
    aiColor3D matColor;

    mat->Get(AI_MATKEY_NAME, matName);
    std::cout << "Material name : " << matName.C_Str() << std::endl;

    std::cout << "\tcolor :" << std::endl;
    mat->Get(AI_MATKEY_COLOR_AMBIENT, matColor);
    returnColor.ambient = glm::vec3(matColor.r, matColor.g, matColor.b);
    Utility::PrintVector("\t    ambient ", glm::vec3(matColor.r, matColor.g, matColor.b));

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, matColor);
    returnColor.diffuse = glm::vec3(matColor.r, matColor.g, matColor.b);
    Utility::PrintVector("\t    diffuse ", glm::vec3(matColor.r, matColor.g, matColor.b));

    mat->Get(AI_MATKEY_COLOR_SPECULAR, matColor);
    returnColor.specular = glm::vec3(matColor.r, matColor.g, matColor.b);
    Utility::PrintVector("\t    specular ", glm::vec3(matColor.r, matColor.g, matColor.b));

    std::cout << std::endl;
    
    return returnColor;
}
