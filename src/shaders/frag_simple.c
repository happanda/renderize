#version 330 core

in vec3 vertColor;
in vec2 texCoord;
out vec4 color;

uniform sampler2D texData1;
uniform sampler2D texData2;
uniform float mixCoeff;

void main()
{
    color = mix(texture(texData1, texCoord), texture(texData2, texCoord), mixCoeff);
}