#include <utility>
#include "render_buffer.h"

using std::swap;


RenderBuffer::RenderBuffer()
    : mRBO(0)
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
}

RenderBuffer const& RenderBuffer::operator=(RenderBuffer&& rhs)
{
    swap(mRBO, rhs.mRBO);
    return *this;
}

void RenderBuffer::create(GLsizei width, GLsizei height, InternalFormat fmt)
{
    free();

    glGenRenderbuffers(1, &mRBO);
    bind();
    GLint internalFormat = 0;
    switch (fmt)
    {
        case InternalFormat::Color:
            internalFormat = GL_RGBA;
            break;
        case InternalFormat::Depth:
            internalFormat = GL_DEPTH_COMPONENT;
            break;
        case InternalFormat::Stencil:
            internalFormat = GL_STENCIL;
            break;
        case InternalFormat::DepthStencil:
            internalFormat = GL_DEPTH_STENCIL;
            break;
    }
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}

RenderBuffer::operator GLuint() const
{
    return mRBO;
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
    }
}
