#version 330 core

out vec4 color;

uniform vec3 iResolution;
uniform float iGlobalTime;

const float PI = 3.1415926535,
            PI2 = PI / 2.;
const vec2 Grid = vec2(20.);

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



#define hsv2rgb(H, S, V)  V * mix(vec3(1.), clamp(abs(fract(H + vec3(3, 2, 1) / 3) * 6. - 3.) - 1., 0., 1.), S)

#define CS(p) vec2(cos(p), sin(p))

#define noise2vec(noise, pos) CS((noise + iGlobalTime * cos(pos.x) * sin(pos.y)) * PI * 2.)

#define poly6rp(val0, val1, t) mix(val0, val1, t * t * t * (t * (t * 6. - 15.) + 10.))

float perlin(vec2 pos, float gridStep)
{
    vec2 pLL = floor(pos / gridStep);
    vec2 gp, dpos, dpos0, eps=vec2(1, 0);
    float dots[4];
    
    gp = gridStep * (pLL);  dpos0 = dpos = (pos - gp) / gridStep; dots[0] = dot(dpos, noise2vec(snoise(gp), gp));
    gp = gridStep * (pLL + eps.xy); dpos = (pos - gp) / gridStep; dots[1] = dot(dpos, noise2vec(snoise(gp), gp));
    gp = gridStep * (pLL + eps.yx); dpos = (pos - gp) / gridStep; dots[2] = dot(dpos, noise2vec(snoise(gp), gp));
    gp = gridStep * (pLL + eps.xx); dpos = (pos - gp) / gridStep; dots[3] = dot(dpos, noise2vec(snoise(gp), gp));

    float D1 = poly6rp(dots[0], dots[1], dpos0.x);
    float D2 = poly6rp(dots[2], dots[3], dpos0.x);
    return poly6rp(D1, D2, dpos0.y);
}

void main()
{
    float H = 0.,
          S = 0.,
          V = 0.;    
    vec2 gridStep = iResolution.xy / (Grid - 1.);
    
    float val = (perlin(gl_FragCoord.xy, gridStep) + 1.) / 2.;
    H = mod(val * 3. + iGlobalTime, 1.);

    float middleVal = (cos(iGlobalTime * 2.) + 1.) / 2.;
    float theta = abs(mod(H, 1.) - middleVal);
    float eps = 0.05 + (sin(iGlobalTime / 2) / 512.);
    S = V = (theta > eps) ? (eps / theta) : 0.85;
    color = vec4(hsv2rgb(H, S, V), 1.);
}