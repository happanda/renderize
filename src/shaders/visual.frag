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
    
    float st = subTime(curTime, periodOn, periodOff);
    if (blink(curTime, periodOn, periodOff))
        R = G = B = (st / (periodOn + periodOff));
    
    
    color = vec4(R, G, B, 1.0f);
}