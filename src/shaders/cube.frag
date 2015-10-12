#version 330 core
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
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

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 color;

uniform Material material;
uniform DirLight dirLight;
uniform PLight pLight;
uniform vec3 viewerPos;

vec4 compDirLight()
{
    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, TexCoords));
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * dirLight.diffuse * vec3(texture(material.diffuse, TexCoords));
    
    vec3 viewDir = normalize(viewerPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = spec * dirLight.specular * vec3(texture(material.specular, TexCoords));
    
    return vec4(ambient + diffuse + specular, 1.0f);
}

vec4 compPLight()
{
    vec3 ambient = pLight.ambient * vec3(texture(material.diffuse, TexCoords));
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(pLight.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * pLight.diffuse * vec3(texture(material.diffuse, TexCoords));
    
    vec3 viewDir = normalize(viewerPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = spec * pLight.specular * vec3(texture(material.specular, TexCoords));
    
    float distance = length(pLight.position - FragPos);
    float attenuation = 1.0f / (pLight.constCoeff + pLight.linCoeff * distance + pLight.quadCoeff * distance * distance);

    return vec4((ambient + diffuse + specular) * attenuation, 1.0f);
}


void main()
{
    color = compPLight();//compDirLight() + compPLight();
}