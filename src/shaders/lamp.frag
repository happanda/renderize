#version 330 core
struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
out vec4 color;

uniform Light light;

void main()
{
    color = vec4(light.diffuse, 1.0f);
}