#pragma once
#include <memory>


struct FrameBuffer;
struct RenderBuffer;
struct RenderTarget;
struct Texture;
struct UniformBuffer;


typedef std::shared_ptr<FrameBuffer>   FrameBufferPtr;
typedef std::shared_ptr<RenderBuffer>  RenderBufferPtr;
typedef std::shared_ptr<RenderTarget>  RenderTargetPtr;
typedef std::shared_ptr<Texture>       TexturePtr;
typedef std::shared_ptr<UniformBuffer> UniformBufferPtr;
