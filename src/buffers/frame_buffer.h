#pragma once
#include <GL/glew.h>


struct FrameBuffer
{
    FrameBuffer();
    ~FrameBuffer();
    
    FrameBuffer(FrameBuffer&& rhs);
    FrameBuffer const& operator=(FrameBuffer&& rhs);
    FrameBuffer(FrameBuffer const&) = delete;
    FrameBuffer const& operator=(FrameBuffer const&) = delete;

    void attachColorTexture2D(GLsizei width, GLsizei height, GLenum target);
    void attachDepthTexture2D(GLsizei width, GLsizei height, GLenum target);
    void attachColorRenderbuffer2D(GLsizei width, GLsizei height, GLenum target);
    void attachDepthRenderbuffer2D(GLsizei width, GLsizei height, GLenum target);

    bool create(GLsizei width, GLsizei height);

    bool isComplete() const;

    operator GLuint() const;

    void bind() const;
    void bindRead() const;
    void bindWrite() const;
    void unbind() const;

    void bindTex() const;
    void unbindTex() const;
    void activeTex(GLenum textureSlot) const;

private:
    

    GLuint mFBO;
    GLuint mColTex;
    GLuint mDepthTex;
    GLuint mStencilTex;
    GLuint mColBuf;
    GLuint mDepthBuf;
    GLuint mStencilBuf;
};
