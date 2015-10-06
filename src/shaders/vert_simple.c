#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out vec3 vertColor;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vertColor = vec3(0.0f, 0.0f, 0.0f);
    texCoord = vec2(texCoords.x, 1.0f - texCoords.y);
}