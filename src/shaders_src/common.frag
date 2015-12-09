#version 330 core

vec4 fragCoord = gl_FragCoord;

out vec4 fragColor;

uniform vec3 iResolution;
uniform float iGlobalTime;
uniform vec4 iDate;

const float M_PI = 3.1415926535,
M_2PI = M_PI * 2.0,
M_PI2 = M_PI / 2.0;

float timeFactor = 1.0;
float time = iGlobalTime * timeFactor;
vec2 Center = iResolution.xy / 2.0;
float Aspect = iResolution.y / iResolution.x;
vec2 Frag = fragCoord.xy;
vec2 cFrag = Frag - Center;
vec2 UV = Frag / iResolution.xy;
vec2 cUV = cFrag / Center;

float cTime = floor(time);
float fTime = fract(time);

#define CS(p) vec2(cos(p), sin(p))
#define DECART(p) (p.x * CS(p.y))
#define POLAR(d) vec2(length(d), atan(d.y, d.x))
#define PRESSENCE(pos, rad) (rad - clamp(dot(pos.xy - Frag, pos.xy - Frag), 0.0, rad))
#define cPRESSENCE(pos, rad) (rad - clamp(dot(pos.xy - cFrag, pos.xy - cFrag), 0.0, rad))
float pressence(vec2 pos, float rad)
{
    vec2 dif = pos - cFrag;
    return (rad - clamp(dot(dif, dif), 0.0, rad));
}

//#define UV (Frag / iResolution.xy);
//#define cUV ((Frag - Center) / Center);

#define SimpleNoise(v) fract(sin(dot(v.xy, vec2(12.9898, 78.233))) * 43758.5453)

#define TRANSIT(val, min0, max0, min1, max1)  ((val - min0) / (max0 - min0) * (max1 - min1) + min1)

#define SINN(x)  ((sin(x) + 1.0) / 2.0)
#define COSN(x)  ((cos(x) + 1.0) / 2.0)

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

float rectReg(vec2 ul, vec2 dim, vec2 pos)
{
    vec2 res = step(ul, pos) * step(1.0 - dim - ul, 1.0 - pos);
    return res.x * res.y;
}

float rect(vec2 ul, vec2 dim, vec2 thickness, vec2 pos)
{
    float outer = rectReg(ul, dim, pos);
    float inner = rectReg(ul + thickness, dim - thickness - thickness, pos);
    return (1.0 - inner) * outer;
}

float circle(vec2 center, float rad, float smooth, vec2 pos)
{
    return smoothstep(rad - smooth, rad + smooth, distance(pos, center));
}







