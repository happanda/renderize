#version 330 core

out vec4 color;

uniform vec2 sourcePos;
uniform float dt;
uniform float curTime;
uniform vec3 randColor;
float PI = 3.1415926535f;
float PI2 = PI / 2.0f;

void main()
{
    float dist = length(gl_FragCoord.xy - sourcePos);
    float xdist = length(gl_FragCoord.x - sourcePos.x);
    float ydist = length(gl_FragCoord.y - sourcePos.y);
    float rval = sin(xdist * ydist / 10.0f + curTime * 7.0f / PI);
    float bval = cos(xdist / ydist / 10.0f - curTime * 2.0f / PI);
    //float rval = cos(dist / 10.0f + curTime * 7.0f / PI);
    //float bval = cos(dist / 10.0f - curTime * 2.0f / PI);

    //if (rval >= 0.4f)
    //    rval = 1.0f;
    //else
    //    rval = 0.0f;
    //if (bval >= 0.2f)
    //    bval = 1.0f;
    //else
    //    bval = 0.0f;
    color = vec4(vec3(rval, 0.0f, bval), 1.0f);

    return;


    int xSq = 80;
    int ySq = 80;
    int sqxNum = int((gl_FragCoord.x + sin(gl_FragCoord.x * curTime) - curTime * 10.0f * randColor.x) / xSq);
    int sqyNum = int((gl_FragCoord.y + cos(gl_FragCoord.y * curTime) - curTime * 10.0f * randColor.y) / ySq);

    if (sqxNum % 2 == 0 && sqyNum % 2 == 0)
        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    else if (sqyNum % 2 == 1 && sqxNum % 2 == 0)
        color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    else if (sqxNum % 2 == 1 && sqyNum % 2 == 0)
        color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    else
        color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}