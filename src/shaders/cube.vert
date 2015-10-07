#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vec4 norm = model * vec4(normal, 0.0f);
    Normal = norm.xyz;
    FragPos = (model * vec4(position, 1.0f)).xyz;
}