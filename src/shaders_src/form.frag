#version 330 core

out vec4 color;

uniform vec2 iResolution;
uniform float iGlobalTime;
uniform mat4 pvm;

const float PI = 3.1415926535,
            PI2 = PI / 2.;
const int NumVerts = 8;
vec3 vertices[NumVerts];
const int NumEdges = 24;
int edges[NumEdges];


vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    vertices[0] = vec3(-0.2f, -0.2f, -0.2f);
    vertices[1] = vec3(0.2f, -0.2f, -0.2f);
    vertices[2] = vec3(0.2f, 0.2f, -0.2f);
    vertices[3] = vec3(-0.2f, 0.2f, -0.2f);
    vertices[4] = vec3(-0.2f, -0.2f, 0.2f);
    vertices[5] = vec3(0.2f, -0.2f, 0.2f);
    vertices[6] = vec3(0.2f, 0.2f, 0.2f);
    vertices[7] = vec3(-0.2f, 0.2f, 0.2f);

    edges[0]  = 0;
    edges[1]  = 1;
    edges[2]  = 1;
    edges[3]  = 2;
    edges[4]  = 2;
    edges[5]  = 3;
    edges[6]  = 3;
    edges[7]  = 0;
    edges[8]  = 4;
    edges[9]  = 5;
    edges[10] = 5;
    edges[11] = 6;
    edges[12] = 6;
    edges[13] = 7;
    edges[14] = 7;
    edges[15] = 4;
    edges[16] = 0;
    edges[17] = 4;
    edges[18] = 1;
    edges[19] = 5;
    edges[20] = 2;
    edges[21] = 6;
    edges[22] = 3;
    edges[23] = 7;


    for (int i = 0; i < NumVerts; ++i)
    {
        vertices[i] = vec3(pvm * vec4(vertices[i], 1.0));
    }
    float R = 0.,
          G = 0.,
          B = 0.;
    float H = 0.,
          S = 0.,
          V = 0.;

    vec2 uv = gl_FragCoord.xy / iResolution.xy * 2.0 - 1.0;
    for (int i = 0; i < NumVerts; ++i)
    {
        float dist = length(vertices[i].xy - uv);
        if (dist < 0.002)
        {
            R = G = B = 1.0 - dist;
        }
    }
    for (int i = 0; i < NumEdges; i += 2)
    {
        vec2 P = vertices[edges[i]].xy;
        vec2 Q = vertices[edges[i + 1]].xy;

        if (dot(uv - P, Q - uv) < 0.0)
            continue;

        float a = (Q.y - P.y) / (Q.x - P.x);
        float b = Q.y - a * Q.x;
        float eps = abs(uv.x * a + b - uv.y);
        if (eps <= 0.01)
        {
            R = G = B = 1.0 - eps;
        }
    }

    color = vec4(R, G, B, 1.0);
}