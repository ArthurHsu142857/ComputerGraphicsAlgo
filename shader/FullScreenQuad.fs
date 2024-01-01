#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D worldPoseTexture;
uniform sampler2D normalMapTexture;
uniform sampler2D fluxMapTexture;
uniform sampler2D depthMapTexture;

uniform sampler2D debugColorTexture;
uniform int debugSwitchTexture;

void main()
{
    if (debugSwitchTexture == 1) {
        FragColor = texture(worldPoseTexture, TexCoords);
    }
    else if (debugSwitchTexture == 2) {
        FragColor = texture(normalMapTexture, TexCoords);
    }
    else if (debugSwitchTexture == 3) {
        FragColor = texture(fluxMapTexture, TexCoords);
    }
    else if (debugSwitchTexture == 4) {
        FragColor = vec4(vec3(1.0f - texture(depthMapTexture, TexCoords).r), 1.0f);
    }
    else {
        FragColor = texture(debugColorTexture, TexCoords);
    }
}