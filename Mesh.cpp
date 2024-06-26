#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, Color color, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->color = color;
    this->indices = indices;
    this->textures = textures;

    SetupMesh();
}

void Mesh::Draw(Shader* shader) {
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int normalNr = 0;
    unsigned int heightNr = 0;
    unsigned int cubemapNr = 0;

    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        bool isCubemap = false;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string
        else if (name == "texture_cubemap") {
            number = std::to_string(cubemapNr++); // transfer unsigned int to string
            isCubemap = true;
        }
        // now set the sampler to the correct texture unit
        shader->setInt((name + number).c_str(), i);

        // and finally bind the texture
        if (isCubemap) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, textures[i].id);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
    }

    // Active light textures
    if (!lightTextures.empty()) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, lightTextures[0].id);
        shader->setInt(lightTextures[0].type.c_str(), 4);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, lightTextures[1].id);
        shader->setInt(lightTextures[1].type.c_str(), 5);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, lightTextures[2].id);
        shader->setInt(lightTextures[2].type.c_str(), 6);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, lightTextures[3].id);
        shader->setInt(lightTextures[3].type.c_str(), 7);
    }

    if (glm::length(color.ambient) == 0 && glm::length(color.diffuse) == 0 && glm::length(color.specular) == 0) {
        shader->setVec3("material.ambient", glm::vec3(0.0f));
        shader->setVec3("material.diffuse", glm::vec3(0.0f));
        shader->setVec3("material.specular", glm::vec3(0.0f));
    }
    else {
        shader->setVec3("material.ambient", color.ambient);
        shader->setVec3("material.diffuse", color.diffuse);
        shader->setVec3("material.specular", color.specular);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::AddLightTexture(string textureName, GLuint textureID) {
    Texture texture;
    texture.id = textureID;
    texture.type = textureName;
    lightTextures.emplace_back(texture);
}

void Mesh::SetupMesh() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    // ids
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, mBoneIDs));

    // weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mWeights));
    glBindVertexArray(0);
}