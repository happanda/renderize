#version 330 core

out vec4 color;

const float PI = 3.1415926535f;
const float PI2 = PI / 2.0f;

const uint DropsCount = 100;
const float tail = -15.0f;

uniform float step;
uniform uint sWinWidth;
uniform uint sWinHeight;
uniform float dt;
uniform float curTime;
uniform vec3 randColor;
uniform vec3 drops[DropsCount];

vec2 polar(vec2 dPoint)
{
    return vec2(sqrt(dPoint.x * dPoint.x + dPoint.y * dPoint.y), atan(dPoint.y, dPoint.x));
}

vec2 hpolar(vec2 dPoint)
{
    return vec2((dPoint.x * dPoint.x + dPoint.y * dPoint.y), atan(dPoint.y, dPoint.x));
}

vec2 decart(vec2 pPoint)
{
    return vec2(pPoint.x * cos(pPoint.y), pPoint.x * sin(pPoint.y));
}

bool blink(float time, float periodOn, float periodOff)
{
    float subTime = time - int(floor(time / (periodOn + periodOff))) * (periodOn + periodOff);
    return (subTime > periodOn);
}

float subTime(float time, float periodOn, float periodOff)
{
    float subTime = time - int(floor(time / (periodOn + periodOff))) * (periodOn + periodOff);
    return subTime;
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

void main()
{
    float periodOn = abs(tan(gl_FragCoord.x / sWinWidth * PI * step) * tan(gl_FragCoord.y / sWinHeight * PI * step));
    float periodOff = periodOn;
    
    vec2 screen = vec2(sWinWidth, sWinHeight);
    vec2 scrCenter = screen / 2.0f;
    float maxRadius = length(scrCenter - screen);
    
    vec2 dP = gl_FragCoord.xy - scrCenter;
    vec2 pP = polar(dP);
    
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;
    float H, S, V;
    
    float st = subTime(curTime, periodOn, periodOff);
    if (blink(curTime, periodOn, periodOff))
        H = sin(curTime) * (st / (periodOn + periodOff));
    //H = gl_FragCoord.x / sWinWidth;
    S = 0.7f;
    V = 0.7f;
    
    color = vec4(hsv2rgb(vec3(H, S, V)), 1.0f);
}