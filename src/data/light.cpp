#include "light.h"


DirLight& DirLight::direction(glm::vec3 const& dir)
{
    mDirection = dir;
    return *this;
}

DirLight& DirLight::ambient(glm::vec3 const& amb)
{
    mAmbient = amb;
    return *this;
}

DirLight& DirLight::diffuse(glm::vec3 const& dif)
{
    mDiffuse = dif;
    return *this;
}

DirLight& DirLight::specular(glm::vec3 const& spec)
{
    mSpecular = spec;
    return *this;
}


PointLight::~PointLight()
{
}

PointLight& PointLight::position(glm::vec3 const& pos)
{
    mPosition = pos;
    return *this;
}

PointLight& PointLight::ambient(glm::vec3 const& amb)
{
    mAmbient = amb;
    return *this;
}

PointLight& PointLight::diffuse(glm::vec3 const& dif)
{
    mDiffuse = dif;
    return *this;
}

PointLight& PointLight::specular(glm::vec3 const& spec)
{
    mSpecular = spec;
    return *this;
}

PointLight& PointLight::constCoeff(float coef)
{
    mConstCoeff = coef;
    return *this;
}

PointLight& PointLight::linCoeff(float coef)
{
    mLinCoeff = coef;
    return *this;
}

PointLight& PointLight::quadCoeff(float coef)
{
    mQuadCoeff = coef;
    return *this;
}



SpotLight& SpotLight::position(glm::vec3 const& pos)
{
    mPosition = pos;
    return *this;
}

SpotLight& SpotLight::ambient(glm::vec3 const& amb)
{
    mAmbient = amb;
    return *this;
}

SpotLight& SpotLight::diffuse(glm::vec3 const& dif)
{
    mDiffuse = dif;
    return *this;
}

SpotLight& SpotLight::specular(glm::vec3 const& spec)
{
    mSpecular = spec;
    return *this;
}

SpotLight& SpotLight::direction(glm::vec3 const& dir)
{
    mDirection = dir;
    return *this;
}

SpotLight& SpotLight::constCoeff(float coef)
{
    mConstCoeff = coef;
    return *this;
}

SpotLight& SpotLight::linCoeff(float coef)
{
    mLinCoeff = coef;
    return *this;
}

SpotLight& SpotLight::quadCoeff(float coef)
{
    mQuadCoeff = coef;
    return *this;
}

SpotLight& SpotLight::cutOff(float val)
{
    mCutOff = val;
    return *this;
}

SpotLight& SpotLight::outerCutOff(float val)
{
    mOuterCutOff = val;
    return *this;
}
