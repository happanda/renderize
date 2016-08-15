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

    bool create(std::string const& vertShaderPath, std::string const& fragShaderPath, IncludeCommonCode icc = IncludeCommonCode::No);
    bool create(std::string const& vertShaderPath, std::string const& geomShaderPath, std::string const& fragShaderPath, IncludeCommonCode icc = IncludeCommonCode::No);

    Program& attach(Shader const& sh);
    bool link();
    void use() const;

    GLenum get() const;

    GLstring lastError() const;

    operator GLenum() const;

    Uniform operator[](std::string const& uniName) const;
    Uniform operator[](char const* uniName) const;

    void free();

    static std::string sShadersPath; // TODO: remove this logic from this class later

private:
    std::string fullShaderPath(std::string const& path) const;

    bool create();
    GLenum mProg;
};
