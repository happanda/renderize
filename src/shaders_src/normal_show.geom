#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec3 Normal;
} gs_in[];

uniform float NormalMagnitude;

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
        gl_Position = gl_in[i].gl_Position + vec4(gs_in[i].Normal, 0.0f) * NormalMagnitude;
        EmitVertex();
        EndPrimitive();
    }
}
