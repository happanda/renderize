#pragma once
#include <string>
#include <GL/glew.h>


typedef std::basic_string<GLchar>  GLstring;

struct shader
{
    shader();
    ~shader();

    bool compile(GLstring const& code, GLenum type);

    GLenum get() const;
    GLenum type() const;

    GLstring lastError() const;
    
    operator GLenum() const;

private:
    GLuint mShader;
};
