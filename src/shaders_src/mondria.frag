// Author @patriciogv - 2015
// http://patriciogonzalezvivo.com

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

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

vec3 White = vec3(248. / 256., 241. / 256., 224. / 256.);
vec3 Black = vec3(23. / 256., 28. / 256., 32. / 256.);
vec3 Red = vec3(180. / 256., 34. / 256., 35. / 256.);
vec3 Yellow = vec3(252. / 256., 194. / 256., 48. / 256.);
vec3 Blue = vec3(0. / 256., 94. / 256., 155. / 256.);

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = White;
    
    float red = rectReg(vec2(-0.5, 0.65 - 0.023), vec2(0.7, 1.0), st);
    float yellow = rectReg(vec2(0.94 - 0.023, 0.65 - 0.023), vec2(0.7, 1.0), st);
    float blue = rectReg(vec2(0.74 - 0.023, -0.5), vec2(0.5, 0.07 + 0.5), st);
    float pct11 = rect(vec2(0.2, -0.5), vec2(0.54, 2.0), vec2(0.023), st);
    float pct12 = rect(vec2(0.2, 0.07), vec2(1.5, 0.58), vec2(0.023), st);
    float pct2 = rect(vec2(0.74 - 0.023, -0.5), vec2(0.2, 2.0), vec2(0.023), st);
    float pct3 = rect(vec2(-0.5, 0.65 - 0.023), vec2(2.0, 0.215), vec2(0.023), st);
    float pct4 = rect(vec2(-0.5, 0.65 - 0.023), vec2(0.59, 1.0), vec2(0.023), st);
    
    color = mix(color, Red, red);
    color = mix(color, Yellow, yellow);
    color = mix(color, Blue, blue);
    color = mix(color, Black, pct11);
    color = mix(color, Black, pct12);
    color = mix(color, Black, pct2);
    color = mix(color, Black, pct3);
    color = mix(color, Black, pct4);


    gl_FragColor = vec4(color,1.0);
}