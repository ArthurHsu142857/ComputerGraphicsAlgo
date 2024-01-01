#version 460 core

layout (location = 0) out vec3 outWorldPose;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outFlux;
layout (location = 3) out vec3 outColor;

struct Light {
    vec3 position;
    vec3 color;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform Light light;
uniform Material mat;


void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * mat.diffuse);

    outWorldPose = FragPos;
    outNormal = norm;
    outFlux = vec3(1.0f, 1.0f, 1.0f);
    outColor = diffuse;
}