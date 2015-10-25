#version 330 core

out vec4 color;

uniform vec3 iResolution;
uniform float iGlobalTime;


const float PI = 3.1415926535;
const float PI2 = PI / 2.0;

const float GridX = 20.0;
const float GridY = 20.0;

float
R = 0.0,
G = 0.0,
B = 0.0,
H = 0.0,
S = 0.0,
V = 0.0;



/*******************************************************************/
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 
vec3 mod289(vec3 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
    return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
    const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                       -0.577350269189626,  // -1.0 + 2.0 * C.x
                        0.024390243902439); // 1.0 / 41.0
    // First corner
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v -   i + dot(i, C.xx);

    // Other corners
    vec2 i1;
    //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
    //i1.y = 1.0 - i1.x;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    // x0 = x0 - 0.0 + 0.0 * C.xx ;
    // x1 = x0 - i1 + 1.0 * C.xx ;
    // x2 = x0 - 1.0 + 2.0 * C.xx ;
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;

    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
    + i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;

    // Gradients: 41 points uniformly over a line, mapped onto a diamond.
    // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt( a0*a0 + h*h );
    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

    // Compute final noise value at P
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}
/*******************************************************************/



vec2 decart(vec2 pPoint)
{
    return vec2(pPoint.x * cos(pPoint.y), pPoint.x * sin(pPoint.y));
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

vec2 noise2vec(float noise, vec2 pos)
{
    return normalize(decart(vec2(1.0, (noise + iGlobalTime * cos(pos.x) * sin(pos.y)) * PI * 2.0)));
}

float poly6rp(float val0, float val1, float t)
{
    float val = t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
    return (1.0 - val) * val0 + val * val1;
}

float perlin(vec2 pos, float gridX, float gridY)
{
    float x = floor(pos.x / gridX);
    float y = floor(pos.y / gridY);
    
    vec2 gridPos[4];
    gridPos[0] = vec2(gridX * x      , gridY * y      );
    gridPos[1] = vec2(gridX * (x + 1.0), gridY * y      );
    gridPos[2] = vec2(gridX * x      , gridY * (y + 1.0));
    gridPos[3] = vec2(gridX * (x + 1.0), gridY * (y + 1.0));

    vec2 cellSize = vec2(gridX, gridY);
    vec2 dPos[4];
    for (int i = 0; i < 4; ++i)
    {
        dPos[i] = (pos - gridPos[i]) / cellSize;
    };
    
    float dots[4];
    dots[0] = dot(dPos[0], noise2vec(snoise(gridPos[0]), gridPos[0]));
    dots[1] = dot(dPos[1], noise2vec(snoise(gridPos[1]), gridPos[1]));
    dots[2] = dot(dPos[2], noise2vec(snoise(gridPos[2]), gridPos[2]));
    dots[3] = dot(dPos[3], noise2vec(snoise(gridPos[3]), gridPos[3]));

    float D1 = poly6rp(dots[0], dots[1], dPos[0].x);
    float D2 = poly6rp(dots[2], dots[3], dPos[0].x);
    return poly6rp(D1, D2, dPos[0].y);
}

void main()
{
    vec2 screen = iResolution.xy;
    vec2 scrCenter = screen / 2.0;
    
    float gridX = iResolution.x / (GridX - 1.0);
    float gridY = iResolution.y / (GridY - 1.0);
    
    float val = perlin(gl_FragCoord.xy, gridX, gridY);
    val = (val + 1.0) / 2.0;
    H = mod(val * 3.0 + iGlobalTime, 1.0);
    
    S = 0.85;
    V = 0.85;
    float middleVal = (cos(iGlobalTime * 2.0) + 1.0) / 2.0;
    float theta = abs(mod(H, 1.0) - middleVal);
    float eps = 0.01 + (sin(iGlobalTime / 2) / 512.0);
    if (theta > eps)
    {
        S = V = eps / theta;
    }
    color = vec4(hsv2rgb(vec3(H, S, V)), 1.0);
}