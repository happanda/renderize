#version 330 core

out vec4 fragColor;

uniform vec3 iResolution;
uniform float iGlobalTime;

vec4 fragCoord;

float timeFactor = 2.0;
float time = iGlobalTime * timeFactor;
float aspect = iResolution.y / iResolution.x;
vec2 center = iResolution.xy / 2.0;
vec2 frag;
vec2 cFrag;


float cTime = floor(time);
float fTime = fract(time);
const int NumStars = 100;
const float NumStarsF = float(NumStars);
float Radius = 5.0;
float Intensity = 0.8;

const float M_PI = 3.1415926535,
            M_2PI = M_PI * 2.0,
            M_PI2 = M_PI / 2.0;

float transit(float min0, float max0, float min1, float max1, float val)
{
    return (val - min0) / (max0 - min0) * (max1 - min1) + min1;
}

#define CS(p) vec2(cos(p), sin(p))

float snoise(vec2 v)
{
    return fract(sin(dot(v.xy, vec2(12.9898,78.233))) * 43758.5453);
}


vec2 decart(vec2 polar)
{
    return polar.x * CS(polar.y);
}

vec2 polar(vec2 dec)
{
    return vec2(length(dec), abs(dec.x) > 0.00001 ? atan(dec.y / dec.x)
        : (dec.y >= 0.0 ? M_PI2 : -M_PI2));
}

float pressence(vec3 pos, float rad)
{
    vec2 dif = pos.xy - cFrag;
    return (rad - clamp(dif.x * dif.x + dif.y * dif.y, 0.0, rad));
}

void main()
{
    fragCoord = gl_FragCoord;
    frag = fragCoord.xy;
    cFrag = frag - center;
    
    mat4 pMat = mat4(0.0);
    float fov = M_PI2;
    float tanFov = tan(fov / 2.0);
    const float near = -1.0;
    const float far = 1.0;

    pMat[0][0] = 1.0 / (aspect * tanFov);
    pMat[1][1] = 1.0 / tanFov;
    pMat[2][2] = -(near + far) / (far - near);
    pMat[2][3] = -1.0;
    pMat[3][2] = 2.0 * near * far / (far - near);
    
    float centLen = length(center.y);
    float R = 0.0,
          G = 0.0,
          B = 0.0;
    
    int Seconds = 60;
    int Minutes = 60;
    float SecondsF = float(Seconds);
    float MinutesF = float(Minutes);
    int timeS = int(time) % Seconds;
    int timeM = (int(time) / Seconds) % Minutes;

    float fracT = fract(time);
    float fracP = fracT * M_PI + M_PI * float(timeS % 2);
    for (int i = 0; i < Seconds; ++i)
    {
        float deltaR = 0.0;
        float deltaA = 0.0;
        
        float angle = M_2PI / SecondsF * (SecondsF - i + float(Seconds) / 4.0);
        float radius = centLen / 2.0;
        vec2 decar = decart(vec2(radius, angle));
        float intens = Intensity;
        R += pressence(vec3(decar.xy, 0.0), Radius * (i == timeM && !(0 == timeS && time > 1.0) ? 5.0 : 1.0)) * intens;

        if (i == timeS)
        {
            deltaR = sin(fracP) * 25.0f;
            deltaA = fracT * M_2PI / SecondsF;
            decar = decart(vec2(radius + deltaR, angle - deltaA));
            R += pressence(vec3(decar.xy, 0.0), Radius) * intens;
        }

        if (0 == timeS && time > 1.0)
        {
            deltaR = sin(fracP) * 25.0f;
            deltaA = fracT * M_2PI / SecondsF;
            decar = decart(vec2(radius + deltaR, M_2PI / SecondsF * (SecondsF - timeM + 1.0 + float(Seconds) / 4.0) - deltaA));
            R += pressence(vec3(decar.xy, 0.0), Radius * 5.0) * intens;
        }
    }

    fragColor = vec4(R, R, R, 1.);
}