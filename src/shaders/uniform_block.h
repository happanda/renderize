#pragma once
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


    /// Helper for assigning parts of Uniform Buffer
struct UniformBlock
{
    UniformBlock(GLuint bufObject, GLintptr offset);
    

    template<class T>
    void operator=(T&& val)
    {
        bind();
        assign(std::forward(val));
        unbind();
    }

private:
    template <class T>
    void assign(T val)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(val), &val);
    }

    template<>
    void assign(bool val)
    {
        GLint v = static_cast<GLint>(val);
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(v), &v);
    }

    template<>
    void assign(float val)
    {
        GLfloat v = static_cast<GLfloat>(val);
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(v), &v);
    }

    template<>
    void assign(double val)
    {
        GLdouble v = static_cast<GLdouble>(val);
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(v), &v);
    }

    template<>
    void assign(int val)
    {
        GLint v = static_cast<GLint>(val);
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(v), &v);
    }

    template<>
    void assign(unsigned int val)
    {
        GLuint v = static_cast<GLuint>(val);
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(v), &v);
    }


    /*=========== vec2 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tvec2<Real, P> const& vec)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(vec), glm::value_ptr(vec));
    }
    
    
    /*=========== vec3 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tvec3<Real, P> const& vec)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(vec), glm::value_ptr(vec));
    }


    /*=========== vec4 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tvec4<Real, P> const& vec)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(vec), glm::value_ptr(vec));
    }


    /*=========== tmat3x3 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tmat3x3<Real, P> const& mat)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(mat), glm::value_ptr(mat));
    }


    /*=========== tmat4x4 ===========*/
    template<glm::precision P, class Real>
    void assign(glm::tmat4x4<Real, P> const& mat)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, mOffset, sizeof(mat), glm::value_ptr(mat));
    }

private:
    void bind();
    void unbind();

    GLuint mBO;
    GLintptr mOffset;
};
