#version 330 core

out vec4 color;

uniform vec3 iResolution;
uniform float iGlobalTime;

const float PI = 3.1415926535,
            PI2 = PI / 2.;
const vec2 Grid = vec2(20.);

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

#define noise2vec(noise, pos) CS((noise + iGlobalTime * cos(pos.x) * sin(pos.y)) * PI * 2.)

#define poly6rp(val0, val1, t) mix(val0, val1, t * t * t * (t * (t * 6. - 15.) + 10.))

float perlin(vec2 pos, vec2 gridStep)
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
    float eps = 0.05 + (sin(iGlobalTime / 2.) / 512.);
    S = V = (theta > eps) ? (eps / theta) : 0.85;
    color = vec4(hsv2rgb(vec3(H, S, V)), 1.);
}