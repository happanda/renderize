#version 330 core

#define NumDirLights 3
#define NumPointLights 3
#define NumSpotLights 3

in vec3 FragPos;
in vec3 Normal;
out vec4 color;

uniform vec3 viewerPos;
uniform samplerCube skyboxTexture;

void main()
{
    vec3 I = normalize(FragPos - viewerPos);
    vec3 R = reflect(I, normalize(Normal));
    color = texture(skyboxTexture, R);
}
