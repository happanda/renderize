#include <fstream>
#include <sstream>
#include <vector>
#include "shader.h"


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

bool Shader::compile(GLstring const& code, GLenum type, IncludeCommonCode inc)
{
    mLastError.clear();

    GLchar const* text = code.c_str();
    std::string common;
    if (inc == IncludeCommonCode::Yes)
    {
        if (type == GL_FRAGMENT_SHADER)
        {
            common = readAllText("Shaders/common.frag");
            if (common.empty())
            {
                mLastError = "Cannot read file Shaders/common.frag";
                return false;
            }
            common += code;
            text = static_cast<GLchar const*>(common.c_str());
        }
    }

    mShader = glCreateShader(type);
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

std::string readAllText(std::string const& path)
{
    std::ifstream ifstr(path);
    if (!ifstr)
        return std::string();
    std::stringstream text;
    ifstr >> text.rdbuf();
    return text.str();
}

void Shader::free()
{
    if (mShader && lastInstance())
    {
        glDeleteShader(mShader);
        mShader = 0;
    }
}
