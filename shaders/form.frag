#version 330 core

in vec4 ColFromPos;
out vec4 color;

uniform vec2 iResolution;
uniform float iGlobalTime;
uniform mat4 pvm;

const float PI = 3.1415926535,
            PI2 = PI / 2.;

vec2 uv = (gl_FragCoord.xy - iResolution.xy / 2.0) / (iResolution.xy / 2.0);


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
    float R = 0.,
          G = 0.,
          B = 0.;
    float H = 0.,
          S = 0.,
          V = 0.;

    color = ColFromPos;
}