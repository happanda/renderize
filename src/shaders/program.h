#pragma once
#include "shader.h"
#include "uniform.h"


struct program
{
    program();
    ~program();

    bool create();
    void attach(shader const& sh);
    bool link();
    void use() const;

    GLenum get() const;

    GLstring lastError() const;

    operator GLenum() const;

    uniform operator[](std::string const& uniName) const;
    uniform operator[](char const* uniName) const;

private:
    GLenum mProg;
};
