#version 330 core

in vec3 FragPos;
in vec3 Normal;
out vec4 color;

uniform vec3 viewerPos;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform int specularShine;

void main()
{
    float ambient = ambientStrength;// * lightColor;
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos.xyz - FragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    float diffuse = diff * diffuseStrength;// * lightColor;
    
    vec3 viewDir = normalize(viewerPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), specularShine);
    float specular = specularStrength * spec;// * lightColor;
    
    color = vec4((ambient + diffuse + specular) * lightColor * objectColor, 1.0f);
}