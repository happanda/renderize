#include "render_target.h"


render_target::render_target()
    : mFBufid(0)
    , mTex(0)
    , mDepthBuf(0)
{
}

render_target::~render_target()
{
    glDeleteRenderbuffers(1, &mDepthBuf);
    glDeleteTextures(1, &mTex);
    glDeleteFramebuffers(1, &mFBufid);
}

bool render_target::create(GLsizei width, GLsizei height)
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

render_target::operator GLuint() const
{
    return mFBufid;
}

void render_target::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFBufid);
}

void render_target::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void render_target::bindTex() const
{
    glBindTexture(GL_TEXTURE_2D, mTex);
}

void render_target::unbindTex() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void render_target::activeTex(GLenum textureSlot) const
{
    glActiveTexture(textureSlot);
    bindTex();
}
