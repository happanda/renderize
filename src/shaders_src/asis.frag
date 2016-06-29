#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D quadTexture;

void main()
{
    color = texture(quadTexture, TexCoords);
    //color = vec4(TexCoords.x, TexCoords.y, 0.5, 1.0);
}
