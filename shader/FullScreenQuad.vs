#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec2 texCoords;
} vs_out;


void main()
{
    gl_Position = vec4(aPos, 1.0); 
    vs_out.texCoords = aTexCoords;
}