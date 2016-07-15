#include <algorithm>
#include <iostream>
#include <utility>
#include "uniform_buffer.h"
#include "shaders/program.h"

using std::cbegin;
using std::cend;
using std::swap;


UniformBuffer::UniformBuffer(std::string const& name)
    : mName(name)
    , mBO(0)
{
}

UniformBuffer::~UniformBuffer()
{
    free();
}

UniformBuffer::UniformBuffer(UniformBuffer&& rhs)
    : mName(std::move(rhs.mName))
    , mBO(0)
{
    swap(mBO, rhs.mBO);
}

UniformBuffer const& UniformBuffer::operator=(UniformBuffer&& rhs)
{
    free();
    mName = std::move(rhs.mName);
    swap(mBO, rhs.mBO);
    return *this;
}

bool UniformBuffer::init(Program const& prog, std::vector<std::string> const& paramNames)
{
    GLuint blockIndex = glGetUniformBlockIndex(prog, mName.c_str());
    if (GL_INVALID_INDEX == blockIndex)
    {
        std::cerr << "glGetUniformBlockIndex return GL_INVALID_INDEX on '" << mName << "'" << std::endl;
        return false;
    }
    GLint blockSize;
    glGetActiveUniformBlockiv(prog, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    mBuffer.resize(blockSize);
    mBuffer.shrink_to_fit();

    glGenBuffers(1, &mBO);
    glBindBuffer(GL_UNIFORM_BUFFER, mBO);
    glBufferData(GL_UNIFORM_BUFFER, blockSize, nullptr, GL_DYNAMIC_DRAW);

    int const numParams = static_cast<int>(paramNames.size());
    std::vector<GLchar const*> names;
    std::transform(cbegin(paramNames), cend(paramNames), std::back_inserter(names),
        [](std::string const& name)
        {
            return static_cast<GLchar const*>(name.c_str());
        });
    std::vector<GLuint> indices(numParams, 0);
    glGetUniformIndices(0, numParams, names.data(), indices.data());
    std::vector<GLint> offsets(numParams, 0);
    glGetActiveUniformsiv(0, numParams, indices.data(), GL_UNIFORM_OFFSET, offsets.data());

    int idx = 0;
    for (auto const& param : paramNames)
    {
        mOffsets[param] = offsets[idx++];
    }
    return true;
}

UniformBlock UniformBuffer::operator[](std::string const& uniName) const
{
    return UniformBlock(mBO, mOffsets.at(uniName));
}

UniformBlock UniformBuffer::operator[](char const* uniName) const
{
    return UniformBlock(mBO, mOffsets.at(uniName));
}

UniformBuffer::operator GLuint() const
{
    return mBO;
}

void UniformBuffer::bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, mBO);
}

void UniformBuffer::unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::free()
{
    mName.clear();
    if (mBO)
    {
        glDeleteBuffers(1, &mBO);
        mBO = 0;
    }
}
