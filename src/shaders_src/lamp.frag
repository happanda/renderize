#version 330 core
out vec4 color;

struct PLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constCoeff;
    float linCoeff;
    float quadCoeff;
};

uniform PLight pLight;


void main()
{
    color = vec4(pLight.diffuse, 1.0f);
}