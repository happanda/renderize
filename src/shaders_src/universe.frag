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
float Radius = 12.0;

const float M_PI = 3.1415926535,
            M_2PI = M_PI * 2.0,
            M_PI2 = M_PI / 2.0;

#define permute(xvec)  mod(((xvec * 34.0) + 1.0) * xvec, 289.0)

float snoise(vec2 v)
{
    return fract(sin(dot(v.xy, vec2(12.9898,78.233))) * 43758.5453);
    /*const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                       -0.577350269189626,  // -1.0 + 2.0 * C.x
                        0.024390243902439); // 1.0 / 41.0
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);
    vec2 i1;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;

    i = mod(i, 289.0);
    vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0))
        + i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
    m = m * m * m * m;

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);*/
}

#define CS(p) vec2(cos(p), sin(p))

vec2 decart(vec2 polar)
{
    return polar.x * CS(polar.y);
}

vec2 polar(vec2 dec)
{
    return vec2(length(dec), abs(dec.x) > 0.00001 ? atan(dec.y / dec.x)
        : (dec.y >= 0 ? M_PI2 : -M_PI2));
}

float pressence(vec3 pos, float rad)
{
    vec2 dif = pos.xy - cFrag;
    return (abs(pos.z)) * (rad - clamp(dif.x * dif.x + dif.y * dif.y, 0.0, rad));
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
    
    float centLen = length(center);
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

        vec3 dPos = vec3(decart(pPolar), 1.0 - locFTime);

        vec4 persPos = pMat * vec4(dPos, 1.0);
        persPos.xy = persPos.xy * centLen / persPos.w;
        float thisRadi = Radius * (locFTime * 0.7 + 0.3);
        R += pressence(persPos.xyz, thisRadi) / thisRadi;
    }
    color = vec4(R, R, R, 1.);
}
