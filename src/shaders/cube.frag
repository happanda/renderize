#version 330 core

in vec3 FragPos;
in vec3 Normal;
out vec4 color;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrenth = 0.3f;
    vec3 ambient = ambientStrenth * lightColor;
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos.xyz - FragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;
    
    color = vec4((ambient + diffuse) * objectColor, 1.0f);
}