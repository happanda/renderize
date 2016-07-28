#include <iostream>
#include <vector>
#include "program.h"

using std::swap;


Program::Program()
    : mProg(0)
{
}

Program::~Program()
{
    if (lastInstance())
        free();
}

Program::Program(Program const& rhs)
    : RefCounted(rhs)
    , mProg(rhs.mProg)
{
}

Program const& Program::operator=(Program const& rhs)
{
    free();
    RefCounted::operator=(rhs);
    mProg = rhs.mProg;
    return *this;
}

bool Program::create(std::string const& vertShaderPath, std::string const& fragShaderPath)
{
    if (!create())
        return false;
    // Shaders
    Shader vertexShader, fragShader;
    if (!vertexShader.compile(readAllText(vertShaderPath), GL_VERTEX_SHADER))
    {
        std::cerr << vertexShader.lastError();
        return false;
    }
    if (!fragShader.compile(readAllText(fragShaderPath), GL_FRAGMENT_SHADER))
    {
        std::cerr << fragShader.lastError();
        return false;
    }
    attach(vertexShader);
    attach(fragShader);
    if (!link())
    {
        std::cerr << lastError();
        return false;
    }
    return true;
}

bool Program::create(std::string const& vertShaderPath, std::string const& geomShaderPath, std::string const& fragShaderPath)
{
    if (!create())
        return false;
    // Shaders
    Shader vertexShader, geomShader, fragShader;
    if (!vertexShader.compile(readAllText(vertShaderPath), GL_VERTEX_SHADER))
    {
        std::cerr << vertexShader.lastError();
        return false;
    }
    if (!geomShader.compile(readAllText(geomShaderPath), GL_GEOMETRY_SHADER))
    {
        std::cerr << geomShader.lastError();
        return false;
    }
    if (!fragShader.compile(readAllText(fragShaderPath), GL_FRAGMENT_SHADER))
    {
        std::cerr << fragShader.lastError();
        return false;
    }
    attach(vertexShader);
    attach(geomShader);
    attach(fragShader);
    if (!link())
    {
        std::cerr << lastError();
        return false;
    }
    return true;
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

void Program::free()
{
    if (mProg)
    {
        glDeleteProgram(mProg);
        mProg = 0;
    }
}

bool Program::create()
{
    free();
    mProg = glCreateProgram();
    return mProg != 0;
}
