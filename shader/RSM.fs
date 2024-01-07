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

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 normal;
} vs_out;

uniform sampler2D texture_diffuse1;
uniform Light light;
uniform Material material;


void main()
{
    vec3 normal = normalize(vs_out.normal);
    vec3 lightDir = normalize(light.position - vs_out.fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * material.diffuse);

    outWorldPose = vs_out.fragPos;
    outNormal = normal;
    // Point light has cosine weight decrease, parallel light don't
    outFlux = vec3(max(dot(normal, lightDir), 0.0f)) * diffuse;
    outColor = diffuse;
}