#pragma once
#include <cstdint>
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
    void createMulti(GLsizei width, GLsizei height, InternalFormat fmt, std::uint8_t samples);

    operator GLuint() const;
    GLint internalFormat() const;

    void bind() const;
    void unbind() const;

private:
    void free();

    GLuint mRBO{ 0 };
    GLint mInternalFormat{ 0 };
    std::uint8_t mNumSamples{ 0 }; // should use some gl request to get it
};
