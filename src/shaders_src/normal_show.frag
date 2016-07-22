#version 330 core

in vec4 geomColor;
out vec4 color;
uniform vec4 uColor;

void main()
{
    color = geomColor;//uColor;
}
