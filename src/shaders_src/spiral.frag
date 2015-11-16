#version 330 core

out vec4 color;

uniform vec3 iResolution;
uniform float iGlobalTime;
uniform float iValue;

float aspect = iResolution.y / iResolution.x;
vec2 center = iResolution.xy / 2.0;
vec2 frag;
vec2 cFrag;
vec2 uv = frag / iResolution.xy;

float time = iGlobalTime;
float cTime = floor(time);
float fTime = fract(time);
const int NumStars = 50;
const float NumStarsF = float(NumStars);
float Radius = 4.0;
float Intensity = 2.0;

const float M_PI = 3.1415926535,
            M_2PI = M_PI * 2.0,
            M_PI2 = M_PI / 2.0;

float snoise(vec2 v)
{
    return fract(sin(dot(v.xy, vec2(12.9898,78.233))) * 43758.5453);
}

#define CS(p) vec2(cos(p), sin(p))

vec2 decart(vec2 polar)
{
    return polar.x * CS(polar.y);
}

float transit(float min0, float max0, float min1, float max1, float val)
{
    return (val - min0) / (max0 - min0) * (max1 - min1) + min1;
}

vec2 polar(vec2 dec)
{
    float len = length(dec);
    if (dec.x == 0.0)
        return vec2(len, dec.y >= 0 ? M_PI2 : 3 * M_PI2);
    return vec2(len, M_PI + atan(dec.y, dec.x));
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    float H = 0.0,
          S = 0.85,
          V = 0.85;
    frag = gl_FragCoord.xy;
    
    float SpiralCount = 10.0;
    for (float i = 0.0; i < SpiralCount; i += 1.0)
    {
        cFrag = frag - center * i / SpiralCount;
        vec2 fragPolar = polar(cFrag);
        float A = 0.0;
        float B = 10.0;
        
        float check = ((fragPolar.x - A) / B - fragPolar.y - time) / M_PI2;
        float pressence = transit(0.0, 1.0, 0.0, iValue, fract(check));
        pressence = 1.0 - clamp(pressence, 0.0, 1.0);
        H = fragPolar.y / M_PI;
        S = V = pressence / (i / SpiralCount + 1.0);
        color += vec4(hsv2rgb(vec3(H, S, V)), 1.);
    }
}
