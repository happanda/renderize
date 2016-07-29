#version 330 core

#define MaxDirLights 3
#define MaxPointLights 3
#define MaxSpotLights 3

struct Material
{
    sampler2D  texDiff1;
    sampler2D  texSpec1;
    sampler2D  texRefl1;
    float shininess;
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
uniform DirLight[MaxDirLights] dirLight;
uniform PointLight[MaxPointLights] pLight;
uniform SpotLight[MaxSpotLights] spLight;
uniform uint numDirLights;
uniform uint numPointLights;
uniform uint numSpotLights;
uniform samplerCube skyboxTexture;

vec4 compDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 compPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 compSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 compReflection(vec3 viewDir, vec3 normal);

void main()
{
    color = vec4(0.0);
    vec3 normal = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewerPos - fs_in.FragPos);
    
    //color = compPointLight(pLight[1], normal, fs_in.FragPos, viewDir);
    //return;
    //color = texture(material.texDiff1, fs_in.TexCoords);
    //return;

    if (DirLightOn)
    {
        vec4 dirLightTotal = vec4(0.0);
        for (uint i = 0u; i < numDirLights; ++i)
        {
            dirLightTotal += compDirLight(dirLight[i], normal, viewDir);
        }
        color += dirLightTotal;
    }
    
    if (PointLightOn)
    {
        vec4 pointLightTotal = vec4(0.0);
        for (uint i = 0u; i < numPointLights; ++i)
        {
            pointLightTotal += compPointLight(pLight[i], normal, fs_in.FragPos, viewDir);
        }
        color += pointLightTotal;
    }
    
    if (SpotLightOn)
    {
        vec4 spotLightTotal = vec4(0.0);
        for (uint i = 0u; i < numSpotLights; ++i)
        {
            spotLightTotal += compSpotLight(spLight[i], normal, fs_in.FragPos, viewDir);
        }
        color += spotLightTotal;
    }

    if (texReflAssigned)
    {
        color += compReflection(viewDir, normal);
    }
}

vec4 compDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
    
    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(normal, -lightDir), 0.0);
    vec4 diffuse = diff * vec4(light.diffuse, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
    
    // Blinn-Phong model specular component
    vec3 halfWay = normalize(-lightDir + viewDir);
    float spec = 0.0;
    if (dot(normal, -lightDir) >= 0.0)
        spec = pow(max(dot(halfWay, normal), 0.0), material.shininess);
    vec4 specular = spec * vec4(light.specular, 1.0) * texture(material.texSpec1, fs_in.TexCoords);
    
    return ambient + diffuse + specular;
}

vec4 compPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lpos = light.position;
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
    
    vec3 lightDir = normalize(fragPos - lpos);
    float diff = max(dot(normal, -lightDir), 0.0);
    vec4 diffuse = diff * vec4(light.diffuse, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
    
    // Blinn-Phong model specular component
    vec3 halfWay = normalize(-lightDir + viewDir);
    float spec = 0.0;
    if (dot(normal, -lightDir) >= 0.0)
        spec = pow(max(dot(halfWay, normal), 0.0), material.shininess);
    vec4 specular = spec * vec4(light.specular, 1.0) * texture(material.texSpec1, fs_in.TexCoords);
    
    float distance = length(lpos - fragPos);
    float attenuation = 1.0 / (light.constCoeff + light.linCoeff * distance + light.quadCoeff * distance * distance);
    
    return (ambient + diffuse + specular) * attenuation;
}

vec4 compSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(fragPos - light.position);
    float theta = dot(-lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intens = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    if (theta > light.outerCutOff)
    {
        vec4 ambient = vec4(light.ambient, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
        
        float diff = max(dot(normal, -lightDir), 0.0);
        
        vec4 diffuse = diff * vec4(light.diffuse, 1.0) * texture(material.texDiff1, fs_in.TexCoords);
        
        // Blinn-Phong model specular component
        vec3 halfWay = normalize(-lightDir + viewDir);
        float spec = 0.0;
        if (dot(normal, -lightDir) >= 0.0)
            spec = pow(max(dot(halfWay, normal), 0.0), material.shininess);
        vec4 specular = spec * vec4(light.specular, 1.0) * texture(material.texSpec1, fs_in.TexCoords);
        
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constCoeff + light.linCoeff * distance + light.quadCoeff * distance * distance);
        
        return (ambient + diffuse + specular) * intens * attenuation;
    }
    else
        return vec4(vec3(0.0), 0.0);
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
