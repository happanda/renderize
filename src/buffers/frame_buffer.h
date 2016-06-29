#pragma once
#include <GL/glew.h>
#include "buffers/fwd.h"


struct FrameBuffer
{
    FrameBuffer();
    ~FrameBuffer();
    
    FrameBuffer(FrameBuffer&& rhs);
    FrameBuffer const& operator=(FrameBuffer&& rhs);
    FrameBuffer(FrameBuffer const&) = delete;
    FrameBuffer const& operator=(FrameBuffer const&) = delete;

    bool create(GLsizei width, GLsizei height);

    void attach(TexturePtr tex);
    void attach(RenderBufferPtr rb);

    bool isComplete() const;

    operator GLuint() const;

    void bind() const;
    void bindRead() const;
    void bindWrite() const;
    void unbind() const;

private:
    void free();

    GLuint mFBO;
};
