#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} gs_in[];

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} gs_out;


void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gs_out.FragPos = gs_in[i].FragPos;
        gs_out.Normal = gs_in[i].Normal;
        gs_out.TexCoords = gs_in[i].TexCoords;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
} 