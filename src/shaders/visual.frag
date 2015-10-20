#version 330 core

out vec4 color;

const uint DropsCount = 100;
const float tail = -15.0f;
uniform uint sWinWidth;
uniform uint sWinHeight;
uniform float dt;
uniform float curTime;
uniform vec3 randColor;
uniform vec3 drops[DropsCount];

bool isMine(vec3 drop)
{
    float posY = gl_FragCoord.y;
    
    float dropY = drop.y * sWinHeight;
    return abs(dropY - posY) <= 2.0f;
}

void main()
{
    vec3 pos = gl_FragCoord.xyz;
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;
    
    for (uint i = uint(0); i < DropsCount; ++i)
    {
        if (!isMine(drops[i]))
            continue;
        float dropX = drops[i].x * sWinWidth;
        float dropY = drops[i].y * sWinHeight;
        float dropZ = drops[i].z;
        
        float dist = length(vec2(dropX, dropY) - pos.xy);
        if (dist < 2.1f)
        {
            float col = exp(dist / tail * (1.0f + dropZ * 2.0f));
            if (dropZ < 0.5f)
                R = R + col * (1.0f - 2.0f * dropZ);
            G = G + col * (1.0f - 2.0f * abs(dropZ - 0.5f));
            if (dropZ > 0.5f)
                B = B + col * (-1.0f + 2.0f * dropZ);
        }
    }
    
    color = vec4(R, G, B, 1.0f);
}