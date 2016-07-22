#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 model;

layout(shared) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 viewerPos;
};

in VS_OUT
{
    vec3 Normal;
} gs_in[];

uniform float NormalMagnitude;
out vec4 geomColor;

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        geomColor = vec4(1.0);
        EmitVertex();
        gl_Position = gl_in[i].gl_Position + gs_in[i].Normal * NormalMagnitude;
        geomColor = vec4(gs_in[i].Normal, 1.0);
        EmitVertex();
        EndPrimitive();
    }
}
