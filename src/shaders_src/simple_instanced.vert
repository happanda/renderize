#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in mat4 instModel;

uniform mat4 model;

layout(shared) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 viewerPos;
};

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    mat4 modelMatrix = model * instModel;

    gl_Position = projection * view * modelMatrix * vec4(position, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(view * modelMatrix)));
    vs_out.Normal = normalize(vec3(projection * vec4(normalMatrix * normal, 1.0)));
    vs_out.FragPos = (modelMatrix * vec4(position, 1.0)).xyz;
    vs_out.TexCoords = texCoords;
}
