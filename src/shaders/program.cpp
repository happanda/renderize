#include <vector>
#include "program.h"


program::program()
    : mProg(0)
{
}

program::~program()
{
    glDeleteProgram(mProg);
}

bool program::create()
{
    mProg = glCreateProgram();
    return mProg != 0;
}

void program::attach(shader const& sh)
{
    glAttachShader(mProg, sh);
}

bool program::link()
{
    if (!mProg)
        return false;
    glLinkProgram(mProg);
    GLint success;
    glGetProgramiv(mProg, GL_LINK_STATUS, &success);
    return success == GL_TRUE;
}

void program::use() const
{
    glUseProgram(mProg);
}

GLenum program::get() const
{
    return mProg;
}

GLstring program::lastError() const
{
    GLint logLen{ 0 };
    glGetProgramiv(mProg, GL_INFO_LOG_LENGTH, &logLen);
    std::vector<GLchar> log;
    log.resize(logLen, '\0');
    glGetProgramInfoLog(mProg, log.size(), nullptr, log.data());
    return GLstring(std::cbegin(log), std::cend(log));
}

program::operator GLenum() const
{
    return mProg;
}

uniform program::operator[](std::string const& uniName) const
{
    return uniform(*this, uniName);
}

uniform program::operator[](char const* uniName) const
{
    return uniform(*this, uniName);
}
