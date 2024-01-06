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

float depthBias = 0.0005;


bool LightSpaceOcclusion(vec3 normal, vec3 lightDir, vec4 fragPosLightSpace)
{
    vec3 projectCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projectCoords = projectCoords * 0.5 + 0.5;

    float closestDepth = texture(depthMapTexture, projectCoords.xy).r;
    float currentDepth = projectCoords.z;

    return (currentDepth - depthBias) > closestDepth;
}

void main()
{
    vec3 normal = normalize(vs_out.normal);
    vec3 lightDir = normalize(light.position - vs_out.fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * material.diffuse);

    bool isShadow = LightSpaceOcclusion(normal, lightDir, vs_out.fragPosLightSpace);
    
    FragColor = vec4(isShadow ? vec3(0.0, 0.0, 0.0) : diffuse, 1.0f);
}