#version 330 core

layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform bool postScene; // drawing after the scene, so set gl_Position.z to pos.w
uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec4 pos = projection * view * vec4(position, 1.0f);
    if (postScene)
        gl_Position = pos.xyww;
    else
        gl_Position = pos;
    TexCoords = position;
}
