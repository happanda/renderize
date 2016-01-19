#pragma once
#include "shader.h"
#include "uniform.h"


struct Program
{
    Program();
    ~Program();

    bool create();
    void attach(Shader const& sh);
    bool link();
    void use() const;

    GLenum get() const;

    GLstring lastError() const;

    operator GLenum() const;

    Uniform operator[](std::string const& uniName) const;
    Uniform operator[](char const* uniName) const;

private:
    GLenum mProg;
};
