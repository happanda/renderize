#version 330 core

#define NumDirLights 3
#define NumPointLights 3
#define NumSpotLights 3

in vec3 FragPos;
in vec3 Normal;
out vec4 color;

layout(shared) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 viewerPos;
};

uniform samplerCube skyboxTexture;
uniform bool reflectOrRefract;
uniform float refractRatio;

void main()
{
    vec3 I = normalize(FragPos - viewerPos);
    vec3 R = vec3(0.0);
    if (reflectOrRefract)
        R = reflect(I, normalize(Normal));
    else
        R = refract(I, normalize(Normal), refractRatio);
    color = texture(skyboxTexture, R);
}
