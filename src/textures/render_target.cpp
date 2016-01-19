#include "render_target.h"


RenderTarget::RenderTarget()
    : mFBufid(0)
    , mTex(0)
    , mDepthBuf(0)
{
}

RenderTarget::~RenderTarget()
{
    glDeleteRenderbuffers(1, &mDepthBuf);
    glDeleteTextures(1, &mTex);
    glDeleteFramebuffers(1, &mFBufid);
}

bool RenderTarget::create(GLsizei width, GLsizei height)
{
    glGenFramebuffers(1, &mFBufid);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBufid);

    glGenTextures(1, &mTex);
    glBindTexture(GL_TEXTURE_2D, mTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glBindTexture(GL_TEXTURE_2D, 0);

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

RenderTarget::operator GLuint() const
{
    return mFBufid;
}

void RenderTarget::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFBufid);
}

void RenderTarget::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::bindTex() const
{
    glBindTexture(GL_TEXTURE_2D, mTex);
}

void RenderTarget::unbindTex() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderTarget::activeTex(GLenum textureSlot) const
{
    glActiveTexture(textureSlot);
    bindTex();
}
