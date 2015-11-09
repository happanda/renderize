#version 330 core

out vec4 color;

uniform vec3 iResolution;
uniform float iGlobalTime;

float aspect = iResolution.y / iResolution.x;
vec2 center = iResolution.xy / 2.0;
vec2 frag = gl_FragCoord.xy;
vec2 cFrag = frag - center;
vec2 uv = frag / iResolution.xy;

float time = iGlobalTime / 4.0;
float cTime = floor(time);
float fTime = fract(time);
const int NumStars = 100;
float Radius = 5.0;

const float M_PI = 3.1415926535,
            M_2PI = M_PI * 2,
            M_PI2 = M_PI / 2.;

#define permute(xvec)  mod(((xvec * 34.0) + 1.0) * xvec, 289.0)

float snoise(vec2 v)
{
    const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
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
    return 130.0 * dot(m, g);
}


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

#define CS(p) vec2(cos(p), sin(p))

//#define noise2vec(noise, pos) CS((noise + iGlobalTime * cos(pos.x) * sin(pos.y)) * M_PI * 2.)
vec2 decart(vec2 polar)
{
    return polar.x * CS(polar.y);
}

float pressence(vec3 pos, float rad)
{
    return pos.z * pos.z * (rad - clamp(length(pos.xy - cFrag), 0.0, rad));
}

void main()
{
    float R = 0.0;
    for (int i = 0; i < NumStars; ++i)
    {
        float locTime = time - (i * 2.0 / NumStars) + i * 600.0;
        float locCTime = floor(locTime);
        float locFTime = fract(locTime);

        vec2 pPolar;
        pPolar.x = snoise(vec2(locCTime + i * 1.0 / NumStars, locCTime + i * 1.0 / NumStars)) * length(center);
        pPolar.y = snoise(vec2(locCTime, locCTime + i * 1.0 / NumStars)) * M_2PI;

        vec3 dPos = vec3(decart(pPolar), locFTime);

        mat4 pMat;
        float fov = M_PI2;
        float tanFov = tan(fov / 2.0);

        vec2 borders = center;
        float near = 0.01;
        float far = 20.0;

        pMat[0][0] = 1.0 / (aspect * tanFov);
        pMat[1][1] = 1.0 / tanFov;
        pMat[2][2] = -(near + far) / (far - near);
        pMat[2][3] = -1.0;
        pMat[3][2] = 2.0 * near * far / (far - near);
        //pMat[3][3] = 0.0;

        vec3 persPos = vec3(pMat * vec4(dPos, dPos.z));
        R += pressence(persPos, Radius) / Radius;
    }
    color = vec4(R, R, R, 1.);
}