#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;

layout(shared) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 viewerPos;
};

out VS_OUT
{
    vec3 Normal;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); 
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.Normal = normalize(vec3(projection * vec4(normalMatrix * normal, 1.0)));
}
