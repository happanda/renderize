#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/vec3.hpp>


struct program;

struct uniform
{
    uniform(program const& prog, std::string const& name);

    operator GLint() const;

    template<glm::precision P, class Real>
    void operator=(glm::tvec2<Real, P> const& vec);

    template<glm::precision P>
    void operator=(glm::tvec2<float, P> const& vec)
    {
        glUniform2fv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void operator=(glm::tvec2<double, P> const& vec)
    {
        glUniform2dv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void operator=(glm::tvec2<int, P> const& vec)
    {
        glUniform2iv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void operator=(glm::tvec2<unsigned int, P> const& vec)
    {
        glUniform2uiv(mLoc, 1, glm::value_ptr(vec));
    }



    template<glm::precision P, class Real>
    void operator=(glm::tvec3<Real, P> const& vec);

    template<glm::precision P>
    void operator=(glm::tvec3<float, P> const& vec)
    {
        glUniform3fv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void operator=(glm::tvec3<double, P> const& vec)
    {
        glUniform3dv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void operator=(glm::tvec3<int, P> const& vec)
    {
        glUniform3iv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void operator=(glm::tvec3<unsigned int, P> const& vec)
    {
        glUniform3uiv(mLoc, 1, glm::value_ptr(vec));
    }

private:
    GLenum mProg;
    GLint mLoc;
};
