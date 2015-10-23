#version 330 core

out vec4 color;

const float PI = 3.1415926535f;
const float PI2 = PI / 2.0f;

const int GridX = 17;
const int GridY = 17;
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

float sinrp(float val0, float val1, float t)
{
    float val = t * t * t * (t * (t * 6 - 15) + 10);
    return (1 - val) * val0 + val * val1;
}

float perlin(vec2 pos, float gridX, float gridY)
{
    int x = int(floor(pos.x / gridX));
    int y = int(floor(pos.y / gridY));
    
    vec2 gridPos[4];
    gridPos[0] = vec2(gridX * x      , gridY * y      );
    gridPos[1] = vec2(gridX * (x + 1), gridY * y      );
    gridPos[2] = vec2(gridX * x      , gridY * (y + 1));
    gridPos[3] = vec2(gridX * (x + 1), gridY * (y + 1));

    vec2 cellSize = vec2(gridX, gridY);
    vec2 dPos[4];
    for (int i = 0; i < 4; ++i)
    {
        dPos[i] = (pos - gridPos[i]) / cellSize;
    };
    
    float dots[4];
    dots[0] = dot(dPos[0], getGrid(x      , y      ));
    dots[1] = dot(dPos[1], getGrid((x + 1), y      ));
    dots[2] = dot(dPos[2], getGrid(x      , (y + 1)));
    dots[3] = dot(dPos[3], getGrid((x + 1), (y + 1)));

    float D1 = sinrp(dots[0], dots[1], dPos[0].x);
    float D2 = sinrp(dots[2], dots[3], dPos[0].x);
    return sinrp(D1, D2, dPos[0].y);//dPos[0].x * dPos[0].y;//
}

void main()
{
    float periodOn = 1.0f;
    float periodOff = periodOn;
    
    vec2 screen = vec2(sWinWidth, sWinHeight);
    vec2 scrCenter = screen / 2.0f;
    float maxRadius = length(scrCenter - screen);
    
    float gridX = float(sWinWidth) / (GridX - 1);
    float gridY = float(sWinHeight) / (GridY - 1);
    
    vec2 dP = gl_FragCoord.xy - scrCenter;
    vec2 pP = polar(dP);
    
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;
    float H, S, V;
    
    float val = perlin(gl_FragCoord.xy, gridX, gridY);
    val = (val + 1.0f) / 2.0f;
    R = val;
    float zPart;
    H = modf(val * 3.0f + curTime, zPart);
    
    S = 0.85f;
    V = 0.85f;
    float middleVal = (cos(curTime * 2.0f) + 1.0f) / 2.0f;
    float theta = abs(modf(H, zPart) - middleVal);// / 2.0f - int(abs(H - middleVal) / 2.0f);
    if (theta > 0.01f)
    {
        S = V = 0.01f / theta;
    }
    R = middleVal;
    color = vec4(vec3(R, R, R), 1.0f);
    color = vec4(hsv2rgb(vec3(H, S, V)), 1.0f);
}