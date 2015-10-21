#version 330 core

out vec4 color;

const float PI = 3.1415926535f;
const float PI2 = PI / 2.0f;

const int GridX = 5;
const int GridY = 5;
const int GridSize = GridX * GridY;

uniform uint sWinWidth;
uniform uint sWinHeight;
uniform float dt;
uniform float curTime;
uniform vec2 grid[GridSize];

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

vec2 getGrid(int i, int j)
{
    return grid[j * GridX + i];
}

float perlin(vec2 pos, float gridX, float gridY)
{
    int i = int(floor(pos.x / gridX));
    int j = int(floor(pos.y / gridY));
    
    vec2 gridPos = vec2(gridX * i, gridY * j);
    vec2 dPos = (pos - gridPos) / vec2(gridX, gridY);
    
    float d1 = dot(dPos, getGrid(i, j));
    float d2 = dot(dPos, getGrid(i + 1, j));
    float d3 = dot(dPos, getGrid(i, j + 1));
    float d4 = dot(dPos, getGrid(i + 1, j + 1));
    
    if (i % 2 == 0)
    {
        dPos.x = 1.0f - dPos.x;
    }
    if (j % 2 == 0)
    {
        dPos.y = 1.0f - dPos.y;
    }
    float D1 = mix(d1, d2, dPos.x);
    float D2 = mix(d3, d4, dPos.x);
    return mix(D1, D2, dPos.y); // dPos.x * dPos.y;
}

void main()
{
    float periodOn = 1.0f;
    float periodOff = periodOn;
    
    vec2 screen = vec2(sWinWidth, sWinHeight);
    vec2 scrCenter = screen / 2.0f;
    float maxRadius = length(scrCenter - screen);
    
    float gridX = float(sWinWidth) / GridX;
    float gridY = float(sWinHeight) / GridY;
    
    vec2 dP = gl_FragCoord.xy - scrCenter;
    vec2 pP = polar(dP);
    
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;
    float H, S, V;
    
    R = H = perlin(gl_FragCoord.xy, gridX, gridY);
    S = 0.99f;
    V = 0.99f;
    
    //color = vec4(vec3(R, R, R), 1.0f);
    color = vec4(hsv2rgb(vec3(H, S, V)), 1.0f);
}