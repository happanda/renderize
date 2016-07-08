#include "light.h"
#include "shaders/program.h"


DirLight& DirLight::direction(glm::vec3 const& dir)
{
    mDirection = dir;
    return *this;
}

glm::vec3 const& DirLight::direction() const
{
    return mDirection;
}

void DirLight::assign(Program& prog, std::string const& name)
{
    prog[name + ".direction"] = direction();
    prog[name + ".ambient"] = ambient();
    prog[name + ".diffuse"] = diffuse();
    prog[name + ".specular"] = specular();
}



PointLight& PointLight::position(glm::vec3 const& pos)
{
    mPosition = pos;
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

glm::vec3 const& PointLight::position() const
{
    return mPosition;
}

float PointLight::constCoeff() const
{
    return mConstCoeff;
}

float PointLight::linCoeff() const
{
    return mLinCoeff;
}

float PointLight::quadCoeff() const
{
    return mQuadCoeff;
}

void PointLight::assign(Program& prog, std::string const& name)
{
    prog[name + ".position"] = position();
    prog[name + ".ambient"] = ambient();
    prog[name + ".diffuse"] = diffuse();
    prog[name + ".specular"] = specular();
    prog[name + ".constCoeff"] = constCoeff();
    prog[name + ".linCoeff"] = linCoeff();
    prog[name + ".quadCoeff"] = quadCoeff();
}


SpotLight& SpotLight::position(glm::vec3 const& pos)
{
    mPosition = pos;
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

SpotLight& SpotLight::direction(glm::vec3 const& dir)
{
    mDirection = dir;
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

glm::vec3 const& SpotLight::position() const
{
    return mPosition;
}

float SpotLight::constCoeff() const
{
    return mConstCoeff;
}

float SpotLight::linCoeff() const
{
    return mLinCoeff;
}

float SpotLight::quadCoeff() const
{
    return mQuadCoeff;
}

glm::vec3 const& SpotLight::direction() const
{
    return mDirection;
}

float SpotLight::cutOff() const
{
    return mCutOff;
}

float SpotLight::outerCutOff() const
{
    return mOuterCutOff;
}

void SpotLight::assign(Program& prog, std::string const& name)
{
    prog[name + ".position"] = position();
    prog[name + ".direction"] = direction();
    prog[name + ".ambient"] = ambient();
    prog[name + ".diffuse"] = diffuse();
    prog[name + ".specular"] = specular();
    prog[name + ".constCoeff"] = constCoeff();
    prog[name + ".linCoeff"] = linCoeff();
    prog[name + ".quadCoeff"] = quadCoeff();
    prog[name + ".cutOff"] = glm::cos(cutOff());
    prog[name + ".outerCutOff"] = glm::cos(outerCutOff());
}
