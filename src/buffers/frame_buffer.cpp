#include <utility>
#include "frame_buffer.h"
#include "buffers/render_buffer.h"
#include "buffers/texture.h"

using std::swap;


FrameBuffer::FrameBuffer()
    : mFBO(0)
{
    glGenFramebuffers(1, &mFBO);
}

FrameBuffer::~FrameBuffer()
{
    free();
}

FrameBuffer::FrameBuffer(FrameBuffer&& rhs)
    : mFBO(0)
{
    swap(mFBO, rhs.mFBO);
}

FrameBuffer const& FrameBuffer::operator=(FrameBuffer&& rhs)
{
    swap(mFBO, rhs.mFBO);
    return *this;
}

void FrameBuffer::attach(Texture& tex)
{
    bind();
    tex.bind();
    GLenum attachment = 0;
    switch (tex.internalFormat())
    {
    case GL_RGBA:
        attachment = GL_COLOR_ATTACHMENT0;
        break;
    case GL_DEPTH_COMPONENT:
        attachment = GL_DEPTH_ATTACHMENT;
        break;
    case GL_STENCIL_INDEX:
        attachment = GL_STENCIL_ATTACHMENT;
        break;
    case GL_DEPTH_STENCIL:
        attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        break;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex, 0);
    tex.unbind();
    unbind();
}

void FrameBuffer::attach(RenderBuffer& rb)
{
    bind();
    rb.bind();
    GLenum attachment = 0;
    switch (rb.internalFormat())
    {
    case GL_RGBA:
        attachment = GL_COLOR_ATTACHMENT0;
        break;
    case GL_DEPTH_COMPONENT:
        attachment = GL_DEPTH_ATTACHMENT;
        break;
    case GL_STENCIL:
        attachment = GL_STENCIL_ATTACHMENT;
        break;
    case GL_DEPTH_STENCIL:
        attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        break;
    }
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rb);
    rb.unbind();
    unbind();
}

bool FrameBuffer::isComplete() const
{
    bind();
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

FrameBuffer::operator GLuint() const
{
    return mFBO;
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void FrameBuffer::bindRead() const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
}

void FrameBuffer::bindWrite() const
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::free()
{
    if (mFBO)
    {
        glDeleteFramebuffers(1, &mFBO);
        mFBO = 0;
    }
}
