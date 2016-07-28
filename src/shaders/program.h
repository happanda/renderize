#pragma once
#include "shader.h"
#include "shaders/fwd.h"
#include "uniform.h"
#include "util/ref_counter.h"


struct Program
    : public RefCounted
{
    Program();
    ~Program();
    Program(Program const&);
    Program const& operator=(Program const&);

    bool create(std::string const& vertShaderPath, std::string const& fragShaderPath);
    bool create(std::string const& vertShaderPath, std::string const& geomShaderPath, std::string const& fragShaderPath);

    void attach(Shader const& sh);
    bool link();
    void use() const;

    GLenum get() const;

    GLstring lastError() const;

    operator GLenum() const;

    Uniform operator[](std::string const& uniName) const;
    Uniform operator[](char const* uniName) const;

    void free();

private:
    bool create();
    GLenum mProg;
};
