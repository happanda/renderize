#pragma once
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include "buffers/fwd.h"
#include "shaders/program.h"


struct RenderTarget
{
    template <class Number, glm::precision P = glm::defaultp>
    RenderTarget(glm::tvec2<Number, P> const& sz, std::uint8_t multiSamples = 0)
        : mRTvao(0)
        , mRTvbo(0)
    {
        init(multiSamples > 0);
        size(glm::ivec2(static_cast<int>(sz.x), static_cast<int>(sz.y)), multiSamples);
    }
    ~RenderTarget();
    
    RenderTarget(RenderTarget&& rhs);
    RenderTarget const& operator=(RenderTarget&& rhs);
    RenderTarget(RenderTarget const&) = delete;
    RenderTarget const& operator=(RenderTarget const&) = delete;

    void init(bool multisampled);
    void size(glm::ivec2 const& size, std::uint8_t multiSamples = 0);

    void draw(glm::ivec2 const& low_left, glm::ivec2 const& size);

    FrameBuffer& fb();
    Texture& tex();
    RenderBuffer& rendbuf();

private:
    void free();

    FrameBufferPtr mFrameBuffer;
    TexturePtr mTexture;
    RenderBufferPtr mRenderBuffer;

    Program mRTprog;
    GLuint mRTvao, mRTvbo;
};
