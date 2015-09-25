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
    if (!mProg)
        return false;
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
    if (!success)
        return false;
}

void program::use()
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
