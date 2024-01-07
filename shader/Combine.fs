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
int sampleRange = 15;
float sampleWeight = 1.0 / pow((sampleRange * 2.0 + 1.0), 2);

bool LightSpaceOcclusion(vec3 normal, vec3 lightDir, vec3 projectCoords)
{
    float closestDepth = texture(depthMapTexture, projectCoords.xy).r;
    float currentDepth = projectCoords.z;

    return (currentDepth - depthBias) > closestDepth;
}

vec3 GlobalLightingEstimate(vec3 currentPointNormal, vec3 currentPointWorldPose, vec3 projectCoords)
{
    vec2 texelSize = 1.0 / textureSize(worldPoseTexture, 0);
    vec3 globalIllumination = vec3(0.0, 0.0, 0.0);

    // Sample in 31 x 31 mask light space
    for (int i = -sampleRange; i <= sampleRange; i++) {
        for (int j = -sampleRange; j <= sampleRange; j++) {
            vec2 coords = projectCoords.xy + vec2(i, j) * texelSize;

            vec3 samplePointWorldPose = texture(worldPoseTexture, coords).rgb;
            vec3 samplePointNormal = texture(normalMapTexture, coords).rgb;
            vec3 samplePointFlux = texture(fluxMapTexture, coords).rgb;

            vec3 samplePointToCurrentPoint = currentPointWorldPose - samplePointWorldPose;
            float distance = length(samplePointToCurrentPoint);
            samplePointToCurrentPoint = normalize(samplePointToCurrentPoint);
            globalIllumination += (
                samplePointFlux * 
                max(0.0, dot(samplePointNormal, samplePointToCurrentPoint)) *
                max(0.0, dot(currentPointNormal, -samplePointToCurrentPoint)) *
                sampleWeight
            );
        }
    }

    return globalIllumination;
}

void main()
{
    vec3 normal = normalize(vs_out.normal);
    vec3 lightDir = normalize(light.position - vs_out.fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * material.diffuse);

    vec3 projectCoords = vs_out.fragPosLightSpace.xyz / vs_out.fragPosLightSpace.w;
    projectCoords = projectCoords * 0.5 + 0.5;
    bool isShadow = LightSpaceOcclusion(normal, lightDir, projectCoords);

    vec3 GI = GlobalLightingEstimate(normal, vs_out.fragPos, projectCoords);
    
    FragColor = vec4(GI + (isShadow ? vec3(0.0, 0.0, 0.0) : diffuse), 1.0f);
}