#pragma once
#include <memory>
#include <string>
#include <glm/vec3.hpp>


struct Program;

    /// Base class for light sources
struct LightBase
{
    //No need in virtual destructor here, no dynamica allocations
    //virtual ~Light()
    //{
    //}

    virtual void assign(Program& prog, std::string const& name) = 0;
};

typedef std::shared_ptr<LightBase> LightPtr;


    /// Base templated class for light sources with some usefull parameters
template <class DerivedLight>
struct Light
    : public LightBase
{
    DerivedLight& ambient(glm::vec3 const& amb)
    {
        mAmbient = amb;
        return static_cast<DerivedLight&>(*this);
    }

    DerivedLight& diffuse(glm::vec3 const& diff)
    {
        mDiffuse = diff;
        return static_cast<DerivedLight&>(*this);
    }

    DerivedLight& specular(glm::vec3 const& spec)
    {
        mSpecular = spec;
        return static_cast<DerivedLight&>(*this);
    }

    glm::vec3 const& ambient() const
    {
        return mAmbient;
    }

    glm::vec3 const& diffuse() const
    {
        return mDiffuse;
    }

    glm::vec3 const& specular() const
    {
        return mSpecular;
    }

private:
    glm::vec3 mAmbient;
    glm::vec3 mDiffuse;
    glm::vec3 mSpecular;
};


    /// Directional light (the sun)
struct DirLight
    : public Light<DirLight>
{
    DirLight& direction(glm::vec3 const& dir);
    glm::vec3 const& direction() const;

    void assign(Program& prog, std::string const& name) override;

private:
    glm::vec3 mDirection;
};


    /// Point light (a lamp)
struct PointLight
    : public Light<PointLight>
{
    PointLight& position(glm::vec3 const& pos);
    PointLight& constCoeff(float coef);
    PointLight& linCoeff(float coef);
    PointLight& quadCoeff(float coef);

    glm::vec3 const& position() const;
    float constCoeff() const;
    float linCoeff() const;
    float quadCoeff() const;

    void assign(Program& prog, std::string const& name) override;

private:
    glm::vec3 mPosition;
    float mConstCoeff;
    float mLinCoeff;
    float mQuadCoeff;
};


    /// Spot light (flash light)
struct SpotLight
    : public Light<SpotLight>
{
    SpotLight& position(glm::vec3 const& pos);
    SpotLight& constCoeff(float coef);
    SpotLight& linCoeff(float coef);
    SpotLight& quadCoeff(float coef);

    SpotLight& direction(glm::vec3 const& dir);
    SpotLight& cutOff(float val);
    SpotLight& outerCutOff(float val);

    glm::vec3 const& position() const;
    float constCoeff() const;
    float linCoeff() const;
    float quadCoeff() const;

    glm::vec3 const& direction() const;
    float cutOff() const;
    float outerCutOff() const;

    void assign(Program& prog, std::string const& name) override;

private:
    glm::vec3 mPosition;
    float mConstCoeff;
    float mLinCoeff;
    float mQuadCoeff;

    glm::vec3 mDirection;
    float mCutOff;
    float mOuterCutOff;
};
