#version 330 core

struct Material
{
    sampler2D  texDiff1;
    sampler2D  texSpec1;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constCoeff;
    float linCoeff;
    float quadCoeff;
};
struct SpotLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constCoeff;
    float linCoeff;
    float quadCoeff;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
//layout (location = 0)
out vec4 color;

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pLight;
uniform SpotLight spLight;
uniform vec3 viewerPos;

vec4 compDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 compPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 compSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewerPos - FragPos);
    color = compDirLight(dirLight, normal, viewDir)
        + compPointLight(pLight, normal, FragPos, viewDir)
        + compSpotLight(spLight, normal, FragPos, viewDir);
}


vec4 compDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.texDiff1, TexCoords));
    
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texDiff1, TexCoords));
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128.0);// TODO: material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.texSpec1, TexCoords));
    
    return vec4(ambient + diffuse + specular, 1.0f);
}

vec4 compPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.texDiff1, TexCoords));
    
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texDiff1, TexCoords));
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128.0);// TODO: material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.texSpec1, TexCoords));
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constCoeff + light.linCoeff * distance + light.quadCoeff * distance * distance);

    return vec4((ambient + diffuse + specular) * attenuation, 1.0f);
}

vec4 compSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intens = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    
    if (theta > light.outerCutOff)
    {
        vec3 ambient = light.ambient * vec3(texture(material.texDiff1, TexCoords));
        
        float diff = max(dot(normal, lightDir), 0.0f);
        vec3 diffuse = diff * light.diffuse * vec3(texture(material.texDiff1, TexCoords));
        
        vec3 viewDir = normalize(viewerPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128.0);// TODO: material.shininess);
        vec3 specular = spec * light.specular * vec3(texture(material.texSpec1, TexCoords));
        
        float distance = length(light.position - fragPos);
        float attenuation = 1.0f / (light.constCoeff + light.linCoeff * distance + light.quadCoeff * distance * distance);
        
        return vec4((ambient + diffuse + specular) * intens * attenuation, 1.0f);
    }
    else
        return vec4(vec3(0.0f), 1.0f);
}