#pragma once
#include <GL/glew.h>
#include "buffers/format.h"
#include "buffers/fwd.h"


struct RenderBuffer
{
    RenderBuffer();
    ~RenderBuffer();
    
    RenderBuffer(RenderBuffer&& rhs);
    RenderBuffer const& operator=(RenderBuffer&& rhs);
    RenderBuffer(RenderBuffer const&) = delete;
    RenderBuffer const& operator=(RenderBuffer const&) = delete;

    void create(GLsizei width, GLsizei height, InternalFormat fmt);

    operator GLuint() const;
    GLint internalFormat() const;

    void bind() const;
    void unbind() const;

private:
    void free();

    GLuint mRBO;
    GLint mInternalFormat;
};
