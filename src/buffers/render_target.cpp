#include <iostream>
#include <utility>
#include "render_target.h"
#include "buffers/frame_buffer.h"
#include "buffers/render_buffer.h"
#include "buffers/texture.h"
#include "util/checked_call.h"

using std::swap;


RenderTarget::~RenderTarget()
{
}

RenderTarget::RenderTarget(RenderTarget&& rhs)
    : mFrameBuffer(std::move(rhs.mFrameBuffer))
{
}

RenderTarget const& RenderTarget::operator=(RenderTarget&& rhs)
{
    mFrameBuffer = std::move(rhs.mFrameBuffer);
    return *this;
}

void RenderTarget::size(glm::ivec2 const& sz, std::uint8_t multiSamples)
{
    if (!mFrameBuffer)
        mFrameBuffer.reset(new FrameBuffer);
    mTexture.reset(new Texture);
    if (multiSamples == 0)
        mTexture->create(sz.x, sz.y, InternalFormat::Color);
    else
        mTexture->createMulti(sz.x, sz.y, InternalFormat::Color, multiSamples);
    mFrameBuffer->attach(*mTexture);
    RenderBuffer renderBuffer;
    if (multiSamples == 0)
        renderBuffer.create(sz.x, sz.y, InternalFormat::Depth);
    else
        renderBuffer.createMulti(sz.x, sz.y, InternalFormat::Depth, multiSamples);
    mFrameBuffer->attach(renderBuffer);
    CHECK(mFrameBuffer->isComplete(), "Error: FrameBuffer is not complete", );
    mFrameBuffer->unbind();
}

FrameBuffer& RenderTarget::fb()
{
    assert(mFrameBuffer && "FrameBuffer is null in RenderTarget");
    return *mFrameBuffer;
}

Texture& RenderTarget::tex()
{
    assert(mTexture && "Texture is null in RenderTarget");
    return *mTexture;
}
