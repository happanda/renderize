#version 330 core

out vec4 color;

uniform float dt;
uniform float curTime;
uniform vec3 randColor;


void main()
{
    int xSq = 80;
    int ySq = 80;
    int sqxNum = int((gl_FragCoord.x + sin(gl_FragCoord.x * curTime)) / xSq);
    int sqyNum = int((gl_FragCoord.y + cos(gl_FragCoord.y * curTime)) / ySq);

    if (sqxNum % 2 == 0 && sqyNum % 2 == 0)
        color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    else if (sqyNum % 2 == 1 && sqxNum % 2 == 0)
        color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    else if (sqxNum % 2 == 1 && sqyNum % 2 == 0)
        color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    else
        color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}