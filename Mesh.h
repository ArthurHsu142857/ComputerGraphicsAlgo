#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utility.h"
#include "Shader.h"

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    glm::vec3 tangent;
    glm::vec3 bitangent;
    //bone indexes which will influence this vertex
    int mBoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float mWeights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct Color {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class Mesh {
public:
    Mesh(vector<Vertex> vertices, Color materials, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader* shader);

    vector<Vertex>       vertices;
    Color                color;
    vector<unsigned int> indices;
    vector<Texture>      textures;

    unsigned int VAO;

private:
    void SetupMesh();

    unsigned int VBO, EBO;
};

