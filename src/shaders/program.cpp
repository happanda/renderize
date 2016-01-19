#include <vector>
#include "program.h"


Program::Program()
    : mProg(0)
{
}

Program::~Program()
{
    glDeleteProgram(mProg);
}

bool Program::create()
{
    mProg = glCreateProgram();
    return mProg != 0;
}

void Program::attach(Shader const& sh)
{
    glAttachShader(mProg, sh);
}

bool Program::link()
{
    if (!mProg)
        return false;
    glLinkProgram(mProg);
    GLint success;
    glGetProgramiv(mProg, GL_LINK_STATUS, &success);
    return success == GL_TRUE;
}

void Program::use() const
{
    glUseProgram(mProg);
}

GLenum Program::get() const
{
    return mProg;
}

GLstring Program::lastError() const
{
    GLint logLen{ 0 };
    glGetProgramiv(mProg, GL_INFO_LOG_LENGTH, &logLen);
    std::vector<GLchar> log;
    log.resize(logLen, '\0');
    glGetProgramInfoLog(mProg, log.size(), nullptr, log.data());
    return GLstring(std::cbegin(log), std::cend(log));
}

Program::operator GLenum() const
{
    return mProg;
}

Uniform Program::operator[](std::string const& uniName) const
{
    return Uniform(*this, uniName);
}

Uniform Program::operator[](char const* uniName) const
{
    return Uniform(*this, uniName);
}
