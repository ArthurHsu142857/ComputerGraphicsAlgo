#version 460 core

out vec4 FragColor;

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
    vec4 fragPosLightSpace;
} vs_out;

uniform sampler2D texture_diffuse1;
uniform Light light;
uniform Material material;

uniform sampler2D worldPoseTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D fluxMapTexture;
uniform sampler2D depthMapTexture;


void main()
{
    vec3 norm = normalize(vs_out.normal);
    vec3 lightDir = normalize(light.position - vs_out.fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * material.diffuse);
    
    FragColor = vec4(diffuse, 1.0f);
}