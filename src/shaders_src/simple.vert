#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 ColFromPos;

void main()
{
    vec4 mPos = model * vec4(position, 1.0);
    vec4 vPos = view * mPos;
    gl_Position = projection * vPos;
    Normal = mat3(model) * normal;
    FragPos = (model * vec4(position, 1.0)).xyz;
    TexCoords = texCoords;
    ColFromPos = vec4(abs(normalize(1.0 - position)), 1.0);
}