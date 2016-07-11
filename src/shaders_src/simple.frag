#version 330 core

#define NumDirLights 3
#define NumPointLights 3
#define NumSpotLights 3

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
uniform bool DirLightOn;
uniform bool PointLightOn;
uniform bool SpotLightOn;


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
//layout (location = 0)
out vec4 color;

uniform Material material;
uniform DirLight[NumDirLights] dirLight;
uniform PointLight[NumPointLights] pLight;
uniform SpotLight[NumSpotLights] spLight;
uniform vec3 viewerPos;

vec4 compDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 compPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 compSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    color = vec4(0.0);
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewerPos - FragPos);

    if (DirLightOn)
    {
        vec4 dirLightTotal = vec4(0.0);
        for (int i = 0; i < NumDirLights; ++i)
        {
            dirLightTotal += compDirLight(dirLight[i], normal, viewDir);
        }
        color += dirLightTotal;
    }

    if (PointLightOn)
    {
        vec4 pointLightTotal = vec4(0.0);
        for (int i = 0; i < NumPointLights; ++i)
        {
            pointLightTotal += compPointLight(pLight[i], normal, FragPos, viewDir);
        }
        color += pointLightTotal;
    }

    if (SpotLightOn)
    {
        vec4 spotLightTotal = vec4(0.0);
        for (int i = 0; i < NumSpotLights; ++i)
        {
            //spotLightTotal += compSpotLight(spLight[i], normal, FragPos, viewDir);
        }
        color += spotLightTotal;
    }
}


vec4 compDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texDiff1, TexCoords);
    
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec4 diffuse = diff * vec4(light.diffuse, 1.0) * texture(material.texDiff1, TexCoords);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128.0);// TODO: material.shininess);
    vec4 specular = spec * vec4(light.specular, 1.0) * texture(material.texSpec1, TexCoords);
    
    return ambient + diffuse + specular;
}

vec4 compPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texDiff1, TexCoords);
    
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec4 diffuse = diff * vec4(light.diffuse, 1.0) * texture(material.texDiff1, TexCoords);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128.0);// TODO: material.shininess);
    vec4 specular = spec * vec4(light.specular, 1.0) * texture(material.texSpec1, TexCoords);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constCoeff + light.linCoeff * distance + light.quadCoeff * distance * distance);

    return (ambient + diffuse + specular);// *attenuation;
}

vec4 compSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intens = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    
    if (theta > light.outerCutOff)
    {
        vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texDiff1, TexCoords);
        
        float diff = max(dot(normal, lightDir), 0.0f);
        vec4 diffuse = diff * vec4(light.diffuse, 1.0) * texture(material.texDiff1, TexCoords);
        
        vec3 viewDir = normalize(viewerPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128.0);// TODO: material.shininess);
        vec4 specular = spec * vec4(light.specular, 1.0) * texture(material.texSpec1, TexCoords);
        
        float distance = length(light.position - fragPos);
        float attenuation = 1.0f / (light.constCoeff + light.linCoeff * distance + light.quadCoeff * distance * distance);
        
        return (ambient + diffuse + specular) * intens * attenuation;
    }
    else
        return vec4(vec3(0.0f), 0.0f);
}