#version 330 core

#define NumDirLights 3
#define NumPointLights 3
#define NumSpotLights 3

struct Material
{
    sampler2D  texDiff1;
    sampler2D  texSpec1;
    sampler2D  texRefl1;
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

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

//layout (location = 0)
out vec4 color;

layout(shared) uniform camera
{
    mat4 projection;
    mat4 view;
    vec3 viewerPos;
};

uniform Material material;
uniform bool texReflAssigned;
uniform DirLight[NumDirLights] dirLight;
uniform PointLight[NumPointLights] pLight;
uniform SpotLight[NumSpotLights] spLight;
uniform samplerCube skyboxTexture;

vec4 compDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 compPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 compSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 compReflection(vec3 viewDir, vec3 normal);

void main()
{
    //color = texture(material.texDiff1, fs_in.TexCoords);
    //return;
    color = vec4(0.0);
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewerPos - fs_in.FragPos);

    //if (DirLightOn)
    //{
    //    vec4 dirLightTotal = vec4(0.0);
    //    for (int i = 0; i < NumDirLights; ++i)
    //    {
    //        dirLightTotal += compDirLight(dirLight[i], normal, viewDir);
    //    }
    //    color += dirLightTotal;
    //}
    
    if (PointLightOn)
    {
        vec4 pointLightTotal = vec4(0.0);
        for (int i = 0; i < NumPointLights; ++i)
        {
            pointLightTotal += compPointLight(pLight[i], normal, fs_in.FragPos, viewDir);
        }
        color += pointLightTotal;
    }
    
    if (SpotLightOn)
    {
        vec4 spotLightTotal = vec4(0.0);
        for (int i = 0; i < NumSpotLights; ++i)
        {
            //spotLightTotal += compSpotLight(spLight[i], normal, fs_in.FragPos, viewDir);
        }
        color += spotLightTotal;
    }

    if (texReflAssigned)
    {
        // TODO: WHY THE FUCK THIS CODE BREAKS MY SHADER WHEN IT ISN'T EVEN CALLED?!
        color += compReflection(viewDir, normal);
    }
}


vec4 compDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
    
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec4 diffuse = diff * vec4(light.diffuse, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
    
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128.0);// TODO: material.shininess);
    vec4 specular = spec * vec4(light.specular, 1.0) * texture(material.texSpec1, fs_in.TexCoords);
    
    return ambient + diffuse + specular;
}

vec4 compPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
    
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec4 diffuse = vec4(light.diffuse, 1.0);// diff * vec4(light.diffuse, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
    return diffuse;
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128.0);// TODO: material.shininess);
    vec4 specular = spec * vec4(light.specular, 1.0) * texture(material.texSpec1, fs_in.TexCoords);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constCoeff + light.linCoeff * distance + light.quadCoeff * distance * distance);

    return (ambient + diffuse + specular);// * attenuation;
}

vec4 compSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intens = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
    
    if (theta > light.outerCutOff)
    {
        vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
        
        float diff = max(dot(normal, lightDir), 0.0f);
        vec4 diffuse = diff * vec4(light.diffuse, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
        
        vec3 viewDir = normalize(viewerPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128.0);// TODO: material.shininess);
        vec4 specular = spec * vec4(light.specular, 1.0) * texture(material.texSpec1, fs_in.TexCoords);
        
        float distance = length(light.position - fragPos);
        float attenuation = 1.0f / (light.constCoeff + light.linCoeff * distance + light.quadCoeff * distance * distance);
        
        return (ambient + diffuse + specular) * intens * attenuation;
    }
    else
        return vec4(vec3(0.0f), 0.0f);
}

vec4 compReflection(vec3 viewDir, vec3 normal)
{
    float reflIntens = texture(material.texRefl1, fs_in.TexCoords).r;
    if (reflIntens > 0.1)
    {
        vec3 R = reflect(-viewDir, normalize(normal));
        return texture(skyboxTexture, R) * reflIntens;
    }
    else
        return vec4(0.0);
}
