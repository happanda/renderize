#version 330 core

out vec4 fragColor;

uniform vec3 iResolution;
uniform float iGlobalTime;

vec4 fragCoord;

float timeFactor = 2.0;
float time = iGlobalTime / timeFactor;
float aspect = iResolution.y / iResolution.x;
vec2 center = iResolution.xy / 2.0;
vec2 frag;
vec2 cFrag;


float cTime = floor(time);
float fTime = fract(time);
const int NumPnts = 13;
const float NumPntsF = float(NumPnts);
float Radius = 6.0;
float Intensity = 0.8;

const float M_PI = 3.1415926535,
            M_2PI = M_PI * 2.0,
            M_PI2 = M_PI / 2.0;

#define CS(p) vec2(cos(p), sin(p))

float transit(float min0, float max0, float min1, float max1, float val)
{
    return (val - min0) / (max0 - min0) * (max1 - min1) + min1;
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
    
    mat4 pMat = mat4(0.0);
    float fov = M_PI2;
    float tanFov = tan(fov / 2.0);
    const float near = 0.0;
    const float far = 10.0;

    pMat[0][0] = 1.0 / (aspect * tanFov);
    pMat[1][1] = 1.0 / tanFov;
    pMat[2][2] = -(near + far) / (far - near);
    pMat[2][3] = -1.0;
    pMat[3][2] = 2.0 * near * far / (far - near);
    
    float centLen = length(center);
    float R = 0.0;
    float xWidth = 16.0;
    float xWidth2 = xWidth / 2.0;
    float zWidth = 20.0;
    
    for (int i = 0; i < NumPnts; ++i)
    {
        float iFl = float(i);
        
        float locTime = time + iFl / NumPntsF;
        float locCTime = floor(locTime);
        float locFTime = locTime - locCTime;
        
        for (int j = 0; j < NumPnts; ++j)
        {
            float jFl = float(j);
            
            float xc = jFl / NumPntsF * xWidth - xWidth2 + sin(iGlobalTime) * xWidth / NumPntsF;
            float zc = (1.0 - iFl / NumPntsF) * zWidth;//(1.0 - locFTime) * 20.0;
            float yc = 2.5 - sin(xc * zc);//4.0 * iFl / NumPntsF * M_PI) * (sin(iGlobalTime) + 1.0) / 2.0;// + iFl / NumPntsF * M_2PI);

            vec4 dPos = vec4(xc, yc, zc, 1.0);
            vec4 persPos = pMat * dPos;
            persPos.xy = persPos.xy * centLen / persPos.w;
            float transVal = transit(0.0, zWidth, 0.3, 1.0, zWidth - zc);
            float intens = Intensity * transVal;
            R += pressence(persPos.xyz, Radius * transVal) * intens;
        }
    }
    fragColor = vec4(R, R, R, 1.);
}
