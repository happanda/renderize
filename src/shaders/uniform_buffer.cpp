#include <algorithm>
#include <iostream>
#include <utility>
#include "uniform_buffer.h"
#include "shaders/program.h"

using std::cbegin;
using std::cend;
using std::swap;

namespace
{
    GLint nextBindingPoint()
    {
        static GLint sBindingPoint = 0;
        return sBindingPoint++;
    }
} // anonymous namespace

UniformBuffer::UniformBuffer(std::string const& name)
    : mName(name)
    , mUBO(0)
    , mBP(nextBindingPoint())
{
}

UniformBuffer::~UniformBuffer()
{
    free();
}

UniformBuffer::UniformBuffer(UniformBuffer&& rhs)
    : mName(std::move(rhs.mName))
    , mUBO(0)
    , mBP(0)
    , mOffsets(std::move(rhs.mOffsets))
{
    swap(mUBO, rhs.mUBO);
    swap(mBP, rhs.mBP);
}

UniformBuffer const& UniformBuffer::operator=(UniformBuffer&& rhs)
{
    free();
    swap(mName, rhs.mName);
    swap(mUBO, rhs.mUBO);
    swap(mBP, rhs.mBP);
    swap(mOffsets, rhs.mOffsets);
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

    glGenBuffers(1, &mUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
    glBufferData(GL_UNIFORM_BUFFER, blockSize, nullptr, GL_DYNAMIC_DRAW);

    int const numParams = static_cast<int>(paramNames.size());
    std::vector<GLchar const*> names;
    std::transform(cbegin(paramNames), cend(paramNames), std::back_inserter(names),
        [](std::string const& name)
        {
            return static_cast<GLchar const*>(name.c_str());
        });
    std::vector<GLuint> indices(numParams, 0);
    glGetUniformIndices(prog, numParams, names.data(), indices.data());
    std::vector<GLint> offsets(numParams, 0);
    glGetActiveUniformsiv(prog, numParams, indices.data(), GL_UNIFORM_OFFSET, offsets.data());

    int idx = 0;
    for (auto const& param : paramNames)
    {
        mOffsets[param] = offsets[idx++];
    }
    return true;
}

UniformBlock UniformBuffer::operator[](std::string const& uniName) const
{
    return UniformBlock(mUBO, mOffsets.at(uniName));
}

UniformBlock UniformBuffer::operator[](char const* uniName) const
{
    return UniformBlock(mUBO, mOffsets.at(uniName));
}

UniformBuffer::operator GLuint() const
{
    return mUBO;
}

void UniformBuffer::bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
}

void UniformBuffer::unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::bind(Program& prog) const
{
    GLuint blockIndex = glGetUniformBlockIndex(prog, mName.c_str());
    glUniformBlockBinding(prog, blockIndex, mBP);
    glBindBufferBase(GL_UNIFORM_BUFFER, mBP, mUBO);
}

void UniformBuffer::free()
{
    mName.clear();
    if (mUBO)
    {
        glDeleteBuffers(1, &mUBO);
        mUBO = 0;
        mBP = 0;
    }
    mOffsets.clear();
}
