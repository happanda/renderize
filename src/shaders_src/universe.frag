#version 330 core

out vec4 color;

uniform vec3 iResolution;
uniform float iGlobalTime;

float aspect = iResolution.y / iResolution.x;
vec2 center = iResolution.xy / 2.0;
vec2 frag;
vec2 cFrag;
vec2 uv = frag / iResolution.xy;

float time = iGlobalTime / 2.0;
float cTime = floor(time);
float fTime = fract(time);
const int NumStars = 50;
const float NumStarsF = float(NumStars);
float Radius = 4.0;
float Intensity = 2.0;

const float M_PI = 3.1415926535,
            M_2PI = M_PI * 2.0,
            M_PI2 = M_PI / 2.0;

#define permute(xvec)  mod(((xvec * 34.0) + 1.0) * xvec, 289.0)

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
    return vec2(length(dec), abs(dec.x) > 0.00001 ? atan(dec.y / dec.x)
        : (dec.y >= 0 ? M_PI2 : -M_PI2));
}

float pressence(vec3 pos, float rad)
{
    vec2 dif = pos.xy - cFrag;
    return rad - clamp(dif.x * dif.x + dif.y * dif.y, 0.0, rad);
}

void main()
{
    frag = gl_FragCoord.xy;
    cFrag = frag - center;
    
    mat4 pMat = mat4(0.0);
    const float fov = M_PI2;
    const float tanFov = tan(fov / 2.0);
    const float near = -1.0;
    const float far = 1.0;

    pMat[0][0] = 1.0 / (aspect * tanFov);
    pMat[1][1] = 1.0 / tanFov;
    pMat[2][2] = -(near + far) / (far - near);
    pMat[2][3] = -1.0;
    pMat[3][2] = 2.0 * near * far / (far - near);
    
    float centLen = length(center.y);
    float R = 0.0;
    for (int i = 0; i < NumStars; ++i)
    {
        float iFl = float(i);
        float locTime = time - (iFl * 2.0 / NumStarsF) + iFl * 600.0;
        float locCTime = floor(locTime);
        float locFTime = locTime - locCTime;

        float shiftTime = locCTime + iFl / NumStarsF;
        vec2 pPolar;
        pPolar.x = snoise(vec2(shiftTime, shiftTime));
        pPolar.y = snoise(vec2(locCTime, shiftTime)) * M_2PI;
        // move a little bit from zero
        pPolar.x = abs(pPolar.x) * 0.9 + 0.02;

        vec4 dPos = vec4(decart(pPolar), 1.0 - locFTime, 1.0);

        vec4 persPos = pMat * dPos;
        persPos.xy = persPos.xy * centLen / persPos.w;
        float transVal = transit(0.0, 1.0, 0.3, 1.0, locFTime);
        float intens = Intensity * transVal;
        R += pressence(persPos.xyz, Radius * transVal) * intens;
    }
    color = vec4(R, R, R, 1.);
}
