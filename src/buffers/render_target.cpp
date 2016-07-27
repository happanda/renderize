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
    , mTexture(std::move(rhs.mTexture))
    , mRenderBuffer(std::move(rhs.mRenderBuffer))
    , mRTvao(0)
    , mRTvbo(0)
{
    swap(mRTvao, rhs.mRTvao);
    swap(mRTvbo, rhs.mRTvbo);
}

RenderTarget const& RenderTarget::operator=(RenderTarget&& rhs)
{
    free();
    mFrameBuffer = std::move(rhs.mFrameBuffer);
    mTexture = std::move(rhs.mTexture);
    mRenderBuffer = std::move(rhs.mRenderBuffer);
    swap(mRTvao, rhs.mRTvao);
    swap(mRTvbo, rhs.mRTvbo);
    return *this;
}

void RenderTarget::init(bool multisampled)
{
    mRTprog = createProgram("shaders/render_target/rt.vert", "shaders/render_target/rt.frag");
    CHECK(mRTprog, "Error creating render target shader program", return;);

    if (!multisampled)
    {
        GLfloat quadVertices[] = {
            // Positions   // TexCoords
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        };
        glGenVertexArrays(1, &mRTvao);
        glGenBuffers(1, &mRTvbo);
        glBindVertexArray(mRTvao);
        glBindBuffer(GL_ARRAY_BUFFER, mRTvbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
        glBindVertexArray(0);
    }
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

    mRenderBuffer.reset(new RenderBuffer);
    if (multiSamples == 0)
        mRenderBuffer->create(sz.x, sz.y, InternalFormat::Depth);
    else
        mRenderBuffer->createMulti(sz.x, sz.y, InternalFormat::Depth, multiSamples);

    mFrameBuffer->attach(*mRenderBuffer);
    CHECK(mFrameBuffer->isComplete(), "Error: FrameBuffer is not complete", );
    mFrameBuffer->unbind();
}

void RenderTarget::draw(glm::ivec2 const& low_left, glm::ivec2 const& size)
{
    if (mRTvao)
    {
        mRTprog.use();
        glBindVertexArray(mRTvao);
        mTexture->active(0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        //glDisableVertexAttribArray(0);
    }
    else // multisampled
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, *mFrameBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, mTexture->size().x, mTexture->size().y, low_left.x, low_left.y, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
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

RenderBuffer& RenderTarget::rendbuf()
{
    assert(mRenderBuffer && "RenderBuffer is null in RenderTarget");
    return *mRenderBuffer;
}

void RenderTarget::free()
{
    mFrameBuffer.reset();
    mTexture.reset();
    mRenderBuffer.reset();
    mRTprog.free();
    if (mRTvbo)
    {
        glDeleteBuffers(1, &mRTvbo);
        mRTvbo = 0;
    }
    if (mRTvao)
    {
        glDeleteVertexArrays(1, &mRTvao);
        mRTvao = 0;
    }
}
