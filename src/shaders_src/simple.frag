#version 330 core

out vec4 color;

uniform sampler2D renderTex0;
uniform sampler2D renderTex1;
uniform vec2 iResolution;

void main()
{
    vec2 vTexCoord = gl_FragCoord.xy / iResolution.xy;

    vec4 leftFrag = texture(renderTex0, vTexCoord);
    leftFrag = vec4(1.0, leftFrag.g, leftFrag.b, 1.0); // Left eye is full red and actual green and blue

    vec4 rightFrag = texture(renderTex1, vTexCoord);
    rightFrag = vec4(rightFrag.r, 1.0, 1.0, 1.0); // Right eye is full green and blue and actual red

    color = vec4(leftFrag.rgb * rightFrag.rgb, 1.0);
}
