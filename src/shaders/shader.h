#pragma once
#include <string>
#include <GL/glew.h>
#include "util/ref_counter.h"


typedef std::basic_string<GLchar>  GLstring;

enum class IncludeCommonCode
{
    Yes,
    No
};


struct Shader
    : public RefCounted
{
    Shader();
    ~Shader();
    Shader(Shader const&);
    Shader const& operator=(Shader const&);

    bool compile(GLstring const& code, GLenum type, IncludeCommonCode inc = IncludeCommonCode::No);

    GLenum get() const;
    GLenum type() const;

    GLstring lastError() const;
    
    operator GLenum() const;

private:
    void free();

    GLuint mShader;
    GLstring mLastError;
};

std::string readAllText(std::string const& path);
