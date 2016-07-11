#pragma once
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include "buffers/fwd.h"


struct RenderTarget
{
    template <class Number, glm::precision P = glm::defaultp>
    RenderTarget(glm::tvec2<Number, P> const& sz)
    {
        size(glm::ivec2(static_cast<int>(sz.x), static_cast<int>(sz.y)));
    }
    ~RenderTarget();
    
    RenderTarget(RenderTarget&& rhs);
    RenderTarget const& operator=(RenderTarget&& rhs);
    RenderTarget(RenderTarget const&) = delete;
    RenderTarget const& operator=(RenderTarget const&) = delete;

    void size(glm::ivec2 const& size);

    FrameBuffer& fb();
    Texture& tex();

private:
    FrameBufferPtr mFrameBuffer;
    TexturePtr mTexture;
};
