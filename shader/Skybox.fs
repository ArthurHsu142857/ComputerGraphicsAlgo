#version 460 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube texture_cubemap0;


void main()
{    
    FragColor = texture(texture_cubemap0, TexCoords);
}