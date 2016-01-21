#pragma once


struct DirLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constCoeff;
    float linCoeff;
    float quadCoeff;
};

struct SpotLight : public PointLight
{
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
};
