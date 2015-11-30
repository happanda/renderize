#pragma once
#include <string>
#include <GL/glew.h>


typedef std::basic_string<GLchar>  GLstring;

enum class IncludeCommonCode
{
    Yes,
    No
};


struct shader
{
    shader();
    ~shader();

    bool compile(GLstring const& code, GLenum type, IncludeCommonCode inc = IncludeCommonCode::No);

    GLenum get() const;
    GLenum type() const;

    GLstring lastError() const;
    
    operator GLenum() const;

private:
    GLuint mShader;
    GLstring mLastError;
};

std::string readAllText(std::string const& path);
