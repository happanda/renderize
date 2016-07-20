#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


struct Program;

struct Uniform
{
    Uniform(Program const& prog, std::string const& name);

    operator GLint() const;


    /*=========== single ===========*/
    template<class Real>
    void operator=(Real&& val)
    {
        glUseProgram(mProg);
        assign(std::forward<Real>(val));
    }

private:
    template <class T>
    void assign(T val);

    template<>
    void assign(bool val)
    {
        glUniform1i(mLoc, static_cast<GLint>(val));
    }

    template<>
    void assign(float val)
    {
        glUniform1f(mLoc, val);
    }

    template<>
    void assign(double val)
    {
        glUniform1d(mLoc, val);
    }

    template<>
    void assign(int val)
    {
        glUniform1i(mLoc, val);
    }

    template<>
    void assign(unsigned int val)
    {
        glUniform1ui(mLoc, val);
    }


    /*=========== vec2 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tvec2<Real, P> const& vec);
    
    template<glm::precision P>
    void assign(glm::tvec2<float, P> const& vec)
    {
        glUniform2fv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void assign(glm::tvec2<double, P> const& vec)
    {
        glUniform2dv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void assign(glm::tvec2<int, P> const& vec)
    {
        glUniform2iv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void assign(glm::tvec2<unsigned int, P> const& vec)
    {
        glUniform2uiv(mLoc, 1, glm::value_ptr(vec));
    }

    
    /*=========== vec3 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tvec3<Real, P> const& vec);

    template<glm::precision P>
    void assign(glm::tvec3<float, P> const& vec)
    {
        glUniform3fv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void assign(glm::tvec3<double, P> const& vec)
    {
        glUniform3dv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void assign(glm::tvec3<int, P> const& vec)
    {
        glUniform3iv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void assign(glm::tvec3<unsigned int, P> const& vec)
    {
        glUniform3uiv(mLoc, 1, glm::value_ptr(vec));
    }


    /*=========== vec4 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tvec4<Real, P> const& vec);

    template<glm::precision P>
    void assign(glm::tvec4<float, P> const& vec)
    {
        glUniform4fv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void assign(glm::tvec4<double, P> const& vec)
    {
        glUniform4dv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void assign(glm::tvec4<int, P> const& vec)
    {
        glUniform4iv(mLoc, 1, glm::value_ptr(vec));
    }

    template<glm::precision P>
    void assign(glm::tvec4<unsigned int, P> const& vec)
    {
        glUniform4uiv(mLoc, 1, glm::value_ptr(vec));
    }


    /*=========== tmat3x3 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tmat3x3<Real, P> const& mat);

    template<glm::precision P>
    void assign(glm::tmat3x3<float, P> const& mat)
    {
        glUniformMatrix3fv(mLoc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    template<glm::precision P>
    void assign(glm::tmat3x3<double, P> const& mat)
    {
        glUniformMatrix3dv(mLoc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    template<glm::precision P>
    void assign(glm::tmat3x3<int, P> const& mat)
    {
        glUniformMatrix3iv(mLoc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    template<glm::precision P>
    void assign(glm::tmat3x3<unsigned int, P> const& mat)
    {
        glUniformMatrix3uiv(mLoc, 1, GL_FALSE, glm::value_ptr(mat));
    }


    /*=========== tmat4x4 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tmat4x4<Real, P> const& mat);

    template<glm::precision P>
    void assign(glm::tmat4x4<float, P> const& mat)
    {
        glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    template<glm::precision P>
    void assign(glm::tmat4x4<double, P> const& mat)
    {
        glUniformMatrix4dv(mLoc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    template<glm::precision P>
    void assign(glm::tmat4x4<int, P> const& mat)
    {
        glUniformMatrix4iv(mLoc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    template<glm::precision P>
    void assign(glm::tmat4x4<unsigned int, P> const& mat)
    {
        glUniformMatrix4uiv(mLoc, 1, GL_FALSE, glm::value_ptr(mat));
    }

private:
    GLenum mProg;
    GLint mLoc;
};
