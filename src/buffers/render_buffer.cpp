#include <utility>
#include "render_buffer.h"

using std::swap;


RenderBuffer::RenderBuffer()
    : mRBO(0)
    , mNumSamples(0)
{
}

RenderBuffer::~RenderBuffer()
{
    free();
}

RenderBuffer::RenderBuffer(RenderBuffer&& rhs)
    : mRBO(0)
{
    swap(mRBO, rhs.mRBO);
    swap(mNumSamples, rhs.mNumSamples);
}

RenderBuffer const& RenderBuffer::operator=(RenderBuffer&& rhs)
{
    swap(mRBO, rhs.mRBO);
    swap(mNumSamples, rhs.mNumSamples);
    return *this;
}

void RenderBuffer::create(GLsizei width, GLsizei height, InternalFormat fmt)
{
    free();
    glGenRenderbuffers(1, &mRBO);
    bind();
    switch (fmt)
    {
        case InternalFormat::Color:
            mInternalFormat = GL_RGBA;
            break;
        case InternalFormat::Depth:
            mInternalFormat = GL_DEPTH_COMPONENT;
            break;
        case InternalFormat::Stencil:
            mInternalFormat = GL_STENCIL;
            break;
        case InternalFormat::DepthStencil:
            mInternalFormat = GL_DEPTH24_STENCIL8;
            break;
    }
    glRenderbufferStorage(GL_RENDERBUFFER, mInternalFormat, width, height);
}

void RenderBuffer::createMulti(GLsizei width, GLsizei height, InternalFormat fmt, std::uint8_t samples)
{
    free();
    mNumSamples = samples;
    glGenRenderbuffers(1, &mRBO);
    bind();
    switch (fmt)
    {
    case InternalFormat::Color:
        mInternalFormat = GL_RGBA;
        break;
    case InternalFormat::Depth:
        mInternalFormat = GL_DEPTH_COMPONENT;
        break;
    case InternalFormat::Stencil:
        mInternalFormat = GL_STENCIL;
        break;
    case InternalFormat::DepthStencil:
        mInternalFormat = GL_DEPTH24_STENCIL8;
        break;
    }
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, mNumSamples, mInternalFormat, width, height);
}

RenderBuffer::operator GLuint() const
{
    return mRBO;
}

GLint RenderBuffer::internalFormat() const
{
    return mInternalFormat;
}

void RenderBuffer::bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
}

void RenderBuffer::unbind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::free()
{
    if (mRBO)
    {
        glDeleteRenderbuffers(1, &mRBO);
        mRBO = 0;
        mNumSamples = 0;
    }
}
