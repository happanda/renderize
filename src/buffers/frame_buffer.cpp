#include "frame_buffer.h"


FrameBuffer::FrameBuffer()
    : mFBO(0)
    , mColTex(0)
    , mDepthTex(0)
    , mStencilTex(0)
    , mColBuf(0)
    , mDepthBuf(0)
    , mStencilBuf(0)
{
}

FrameBuffer::~FrameBuffer()
{
    if (mColTex)
        glDeleteTextures(1, &mColTex);
    if (mDepthTex)
        glDeleteTextures(1, &mDepthTex);
    if (mStencilTex)
        glDeleteTextures(1, &mStencilTex);
    if (mColBuf)
        glDeleteRenderbuffers(1, &mColBuf);
    if (mDepthBuf)
        glDeleteRenderbuffers(1, &mDepthBuf);
    if (mStencilBuf)
        glDeleteRenderbuffers(1, &mStencilBuf);
    if (mFBO)
        glDeleteFramebuffers(1, &mFBO);
}

FrameBuffer::FrameBuffer(FrameBuffer&& rhs)
    : mFBO(rhs.mFBO)
    , mColTex(rhs.mColTex)
    , mDepthTex(rhs.mDepthTex)
    , mStencilTex(rhs.mStencilTex)
    , mColBuf(rhs.mColBuf)
    , mDepthBuf(rhs.mDepthBuf)
    , mStencilBuf(rhs.mStencilBuf)
{
    rhs.mFBO = rhs.mColTex = rhs.mDepthTex = rhs.mStencilTex = rhs.mColBuf = rhs.mDepthBuf = rhs.mStencilBuf = 0;
}

FrameBuffer const& FrameBuffer::operator=(FrameBuffer&& rhs)
{
    mFBO = rhs.mFBO;
    mColTex = rhs.mColTex;
    mDepthTex = rhs.mDepthTex;
    mStencilTex = rhs.mStencilTex;
    mColBuf = rhs.mColBuf;
    mDepthBuf = rhs.mDepthBuf;
    rhs.mFBO = rhs.mColTex = rhs.mDepthTex = rhs.mStencilTex = rhs.mColBuf = rhs.mDepthBuf = rhs.mStencilBuf = 0;
    return *this;
}

void FrameBuffer::attachColorTexture2D(GLsizei width, GLsizei height, GLenum target)
{
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(target, mFBO);

    glGenTextures(1, &mTex);
    glBindTexture(GL_TEXTURE_2D, mTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(target, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex, 0);
}

void FrameBuffer::attachDepthTexture2D(GLsizei width, GLsizei height, GLenum target)
{
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(target, mFBO);

    glGenTextures(1, &mTex);
    glBindTexture(GL_TEXTURE_2D, mTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(target, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTex, 0);
}

void FrameBuffer::attachColorRenderbuffer2D(GLsizei width, GLsizei height, GLenum target)
{
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(target, mFBO);

    glGenTextures(1, &mTex);
    glBindTexture(GL_TEXTURE_2D, mTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(target, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex, 0);
}

void FrameBuffer::attachDepthRenderbuffer2D(GLsizei width, GLsizei height, GLenum target)
{
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(target, mFBO);

    glGenTextures(1, &mTex);
    glBindTexture(GL_TEXTURE_2D, mTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(target, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTex, 0);
}

bool FrameBuffer::create(GLsizei width, GLsizei height)
{
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    glGenTextures(1, &mTex);
    glBindTexture(GL_TEXTURE_2D, mTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glGenRenderbuffers(1, &mDepthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuf);
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTex, 0);
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers);

    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
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

void FrameBuffer::bindTex() const
{
    glBindTexture(GL_TEXTURE_2D, mTex);
}

void FrameBuffer::unbindTex() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::activeTex(GLenum textureSlot) const
{
    glActiveTexture(textureSlot);
    bindTex();
}
