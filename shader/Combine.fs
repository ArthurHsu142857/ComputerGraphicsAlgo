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
uniform float randomNumbers[400];

uniform sampler2D worldPoseTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D fluxMapTexture;
uniform sampler2D depthMapTexture;

float depthBias = 0.005f;
float sampleRadius = 1.0f;
float PI_2 = 6.283185;

bool LightSpaceOcclusion(vec3 normal, vec3 lightDir, vec3 projectCoords)
{
    float closestDepth = texture(depthMapTexture, projectCoords.xy).r;
    float currentDepth = projectCoords.z;

    return (currentDepth - depthBias) > closestDepth;
}

vec3 IndirectLightingEstimate(vec3 currentPointNormal, vec3 currentPointWorldPose, vec3 projectCoords)
{
    vec2 texelSize = 1.0 / textureSize(worldPoseTexture, 0);
    vec3 globalIllumination = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < 400; i += 2)
    {
        vec2 coords = vec2(
            projectCoords.x + sampleRadius * randomNumbers[i] * sin(PI_2 * randomNumbers[i + 1]),
            projectCoords.y + sampleRadius * randomNumbers[i] * cos(PI_2 * randomNumbers[i + 1])
        );

        vec3 samplePointWorldPose = texture(worldPoseTexture, coords).rgb;
        vec3 samplePointNormal = normalize(texture(normalMapTexture, coords).rgb);
        vec3 samplePointFlux = texture(fluxMapTexture, coords).rgb;

        vec3 samplePointToCurrentPoint = currentPointWorldPose - samplePointWorldPose;
        float distance = length(samplePointToCurrentPoint);

        if (distance < 4.0f)
            continue;

        samplePointToCurrentPoint = normalize(samplePointToCurrentPoint);
        globalIllumination += (
            samplePointFlux
            * max(0.0, dot(samplePointNormal, samplePointToCurrentPoint))
            * max(0.0, dot(currentPointNormal, -samplePointToCurrentPoint))
            / pow(distance, 4)
        );
    }

    return globalIllumination;
}

void main()
{
    vec3 normal = normalize(vs_out.normal);
    vec3 lightDir = normalize(light.position - vs_out.fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * (diff * material.diffuse);

    // Shadow mapping
    vec3 projectCoords = vs_out.fragPosLightSpace.xyz / vs_out.fragPosLightSpace.w;
    projectCoords = projectCoords * 0.5 + 0.5;
    bool isShadow = LightSpaceOcclusion(normal, lightDir, projectCoords);

    // Reflective shadow map
    vec3 GI = IndirectLightingEstimate(normal, vs_out.fragPos, projectCoords);
    
    FragColor = vec4(GI * 10 + (isShadow ? vec3(0.0, 0.0, 0.0) : diffuse), 1.0f);
}