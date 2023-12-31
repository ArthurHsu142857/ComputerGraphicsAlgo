#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec2 texCoords;
} vs_out;

uniform sampler2D worldPoseTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D fluxMapTexture;
uniform sampler2D depthMapTexture;

uniform sampler2D debugColorTexture;
uniform int debugSwitchTexture;

uniform float nearPlane;
uniform float farPlane;


float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
    if (debugSwitchTexture == 1) {
        FragColor = texture(worldPoseTexture, vs_out.texCoords);
    }
    else if (debugSwitchTexture == 2) {
        FragColor = texture(normalMapTexture, vs_out.texCoords);
    }
    else if (debugSwitchTexture == 3) {
        FragColor = texture(fluxMapTexture, vs_out.texCoords);
    }
    else if (debugSwitchTexture == 4) {
        float depthValue = texture(depthMapTexture, vs_out.texCoords).r;
        FragColor = vec4(vec3(LinearizeDepth(depthValue) / farPlane), 1.0);
    }
    else {
        FragColor = texture(debugColorTexture, vs_out.texCoords);
    }
}