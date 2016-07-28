#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D quadTexture;

void main()
{
    vec4 clr = texture(quadTexture, TexCoords);
    //color = vec4(vec3(0.2126 * clr.r + 0.7152 * clr.g + 0.0722 * clr.b), clr.a);
    color = clr;
}
