#pragma once
#include <glm/vec3.hpp>


struct DirLight
{
    glm::vec3 mDirection;
    glm::vec3 mAmbient;
    glm::vec3 mDiffuse;
    glm::vec3 mSpecular;

    DirLight& direction(glm::vec3 const& dir);
    DirLight& ambient(glm::vec3 const& amb);
    DirLight& diffuse(glm::vec3 const& dif);
    DirLight& specular(glm::vec3 const& spec);
};

struct PointLight
{
    virtual ~PointLight();

    glm::vec3 mPosition;
    glm::vec3 mAmbient;
    glm::vec3 mDiffuse;
    glm::vec3 mSpecular;
    float mConstCoeff;
    float mLinCoeff;
    float mQuadCoeff;

    virtual PointLight& position(glm::vec3 const& pos);
    virtual PointLight& ambient(glm::vec3 const& amb);
    virtual PointLight& diffuse(glm::vec3 const& dif);
    virtual PointLight& specular(glm::vec3 const& spec);
    virtual PointLight& constCoeff(float coef);
    virtual PointLight& linCoeff(float coef);
    virtual PointLight& quadCoeff(float coef);
};

struct SpotLight : public PointLight
{
    glm::vec3 mDirection;
    float mCutOff;
    float mOuterCutOff;

    SpotLight& position(glm::vec3 const& pos) override;
    SpotLight& ambient(glm::vec3 const& amb) override;
    SpotLight& diffuse(glm::vec3 const& dif) override;
    SpotLight& specular(glm::vec3 const& spec) override;
    SpotLight& direction(glm::vec3 const& dir);

    SpotLight& constCoeff(float coef) override;
    SpotLight& linCoeff(float coef) override;
    SpotLight& quadCoeff(float coef) override;
    SpotLight& cutOff(float val);
    SpotLight& outerCutOff(float val);
};
