#pragma once
#include <memory>


struct FrameBuffer;
struct RenderBuffer;
struct Texture;


typedef std::shared_ptr<FrameBuffer>   FrameBufferPtr;
typedef std::shared_ptr<RenderBuffer>  RenderBufferPtr;
typedef std::shared_ptr<Texture>       TexturePtr;
