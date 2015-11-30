#include <fstream>
#include <sstream>
#include <vector>
#include "shader.h"


shader::shader()
    : mShader(0)
{
}

shader::~shader()
{
    glDeleteShader(mShader);
}

bool shader::compile(GLstring const& code, GLenum type, IncludeCommonCode inc)
{
    mLastError.clear();

    GLchar const* text = code.c_str();
    std::string common;
    if (inc == IncludeCommonCode::Yes)
    {
        if (type == GL_FRAGMENT_SHADER)
        {
            common = readAllText("shaders/common.frag");
            if (common.empty())
            {
                mLastError = "Cannot read file shaders/common.frag";
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

GLstring shader::lastError() const
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

GLenum shader::get() const
{
    return mShader;
}

GLenum shader::type() const
{
    GLint type{ 0 };
    glGetShaderiv(mShader, GL_SHADER_TYPE, &type);
    return type;
}

shader::operator GLenum() const
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