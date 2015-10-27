#pragma once
#include <GL/glew.h>


struct render_target
{
    render_target();
    ~render_target();

    bool create(GLsizei width, GLsizei height);

    operator GLuint() const;

    void bind() const;
    void unbind() const;

    void bindTex() const;
    void unbindTex() const;
    void activeTex(GLenum textureSlot) const;

private:
    GLuint mFBufid;
    GLuint mTex;
    GLuint mDepthBuf;
};
