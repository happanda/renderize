#version 330 core

out vec4 fragColor;

uniform vec3 iResolution;
uniform float iGlobalTime;
uniform vec4 iDate;

const float M_PI = 3.1415926535,
M_2PI = M_PI * 2.0,
M_PI2 = M_PI / 2.0;

vec4 fragCoord;

const float timeFactor = 1.0;
float time = iDate.w * timeFactor;
vec2 center = iResolution.xy / 2.0;
vec2 frag;
vec2 cFrag;

float cTime = floor(time);
float fTime = fract(time);
float Radius = 5.0;
float Intensity = 0.3;

#define CS(p) vec2(cos(p), sin(p))
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
    
    float centLen = length(center.y);
    float R = 0.0,
          G = 0.0,
          B = 0.0;
    
    const float Seconds = 60.0;
    const float Minutes = 60.0;
    const float Hours = 12.0;
    float timeS = mod(time, Seconds);
    float timeM = mod(floor(time / Seconds), Minutes);
    float timeH = mod(floor(floor(time / Seconds) / Minutes), Hours);

    float radius = centLen / 2.0;
    for (float i = 0.0; i < Seconds; ++i)
    {
        float angle = M_2PI / Seconds * (Seconds - i + Seconds / 4.0);

        vec2 decar = decart(vec2(radius, angle));
        R += pressence(vec3(decar.xy, 0.0), Radius) * Intensity;
    }

    float secAngle = M_2PI / Seconds * (Seconds - timeS + Seconds / 4.0);
    float secRad = radius + sin(timeS * M_PI) * 25.0;
    vec2 decar = decart(vec2(secRad, secAngle));
    R += pressence(vec3(decar, 0.0), Radius) * Intensity;

    float minAngle = M_2PI / Minutes * (Minutes - timeM + Minutes / 4.0);
    float minRad = radius;
    if (timeS >= 0.0 && timeS < 1.0 && time >= 1.0)
    {
        minAngle += (1.0 - timeS) * M_2PI / Minutes;
        minRad += sin(timeS * M_PI) * 25.0;
    }
    decar = decart(vec2(minRad, minAngle));
    G += pressence(vec3(decar, 0.0), Radius * 4.0) * Intensity;

    float hrAngle = M_2PI / Hours * (Hours - timeH + Hours / 4.0);
    float hrRad = radius;
    if (timeS >= 0.0 && timeS < 1.0 && timeM >= 0.0 && timeM < 1.0 && time >= Seconds)
    {
        hrAngle += (1.0 - timeS) * M_2PI / Hours;
        hrRad += sin(timeS * M_PI) * 25.0;
    }
    decar = decart(vec2(hrRad, hrAngle));
    B += pressence(vec3(decar, 0.0), Radius * 7.0) * Intensity;

    if (G > 0.0)
        B = 0.0;
    if (B > 0.0 || G > 0.0)
        R = 0.0;
    fragColor = vec4(R, G, B, 1.);
}
