#version 330 core

in vec3 TexCoords;
out vec4 color;

uniform samplerCube skyboxTexture;

void main()
{
    color = texture(skyboxTexture, TexCoords);
}
