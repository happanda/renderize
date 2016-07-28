#version 330 core

#define KRN_SZ 9

in vec2 TexCoords;
out vec4 color;

uniform sampler2D quadTexture;
uniform mat3 kernel;
uniform vec2 offset;

void main()
{
    vec2 offsets[KRN_SZ] = vec2[] (
        vec2(-offset.x,  offset.y),
        vec2(      0.0,  offset.y),
        vec2( offset.x,  offset.y),
        vec2(-offset.x,       0.0),
        vec2(      0.0,       0.0),
        vec2( offset.x,       0.0),
        vec2(-offset.x, -offset.y),
        vec2(      0.0, -offset.y),
        vec2( offset.x, -offset.y)
    );
    vec4 clr = vec4(0.0);
    for (int i = 0; i < KRN_SZ; ++i)
    {
        clr += texture(quadTexture, TexCoords + offsets[i]) * kernel[i / 3][i % 3];
    }
    color = clr;
}
