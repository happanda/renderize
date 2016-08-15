#include <vector>
#include "shader.h"
#include "program.h"


Shader::Shader()
    : mShader(0)
{
}

Shader::~Shader()
{
    free();
}

Shader::Shader(Shader const& rhs)
    : RefCounted(rhs)
    , mShader(rhs.mShader)
{
}

Shader const& Shader::operator=(Shader const& rhs)
{
    free();
    RefCounted::operator=(rhs);
    mShader = rhs.mShader;
    return *this;
}

bool Shader::compile(GLstring const& code, ShaderType type)
{
    mLastError.clear();
    GLenum glType = GL_VERTEX_SHADER;
    if (type == ShaderType::Geometry)
        glType = GL_GEOMETRY_SHADER;
    else if (type == ShaderType::Fragment)
        glType = GL_FRAGMENT_SHADER;

    GLchar const* text = code.c_str();
    mShader = glCreateShader(glType);
    if (!mShader)
        return false;

    glShaderSource(mShader, 1, &text, nullptr);
    glCompileShader(mShader);
    // check success
    GLint success{ GL_FALSE };
    glGetShaderiv(mShader, GL_COMPILE_STATUS, &success);
    return success == GL_TRUE;
}

GLstring Shader::lastError() const
{
    if (!mLastError.empty())
        return mLastError;
    GLint logLen{ 0 };
    glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &logLen);
    std::vector<GLchar> log;
    log.resize(logLen, '\0');
    glGetShaderInfoLog(mShader, log.size(), nullptr, log.data());
    return GLstring(std::cbegin(log), std::cend(log));
}

GLenum Shader::get() const
{
    return mShader;
}

GLenum Shader::type() const
{
    GLint type{ 0 };
    glGetShaderiv(mShader, GL_SHADER_TYPE, &type);
    return type;
}

Shader::operator GLenum() const
{
    return mShader;
}

void Shader::free()
{
    if (mShader && lastInstance())
    {
        glDeleteShader(mShader);
        mShader = 0;
    }
}
