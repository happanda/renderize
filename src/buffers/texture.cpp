#include <utility>
#include "texture.h"
#include "shaders/program.h"
#include "util/soil_image.h"

using std::swap;


Texture::Texture()
    : mTex(0)
    , mTarget(0)
    , mInternalFormat(0)
    , mType(TexType::Normal)
    , mNumSamples(0)
{
}

Texture::~Texture()
{
    free();
}

Texture::Texture(Texture&& rhs)
    : mTex(0)
    , mTarget(0)
    , mInternalFormat(0)
    , mType(TexType::Normal)
    , mNumSamples(0)
{
    swap(mTex, rhs.mTex);
    swap(mTarget, rhs.mTarget);
    swap(mInternalFormat, rhs.mInternalFormat);
    swap(mType, rhs.mType);
    swap(mNumSamples, rhs.mNumSamples);
}

Texture const& Texture::operator=(Texture&& rhs)
{
    free();
    swap(mTex, rhs.mTex);
    swap(mTarget, rhs.mTarget);
    swap(mInternalFormat, rhs.mInternalFormat);
    swap(mType, rhs.mType);
    swap(mNumSamples, rhs.mNumSamples);
    return *this;
}

void Texture::create(GLsizei width, GLsizei height, InternalFormat fmt)
{
    free();
    mSize = glm::ivec2(width, height);
    mTarget = GL_TEXTURE_2D;

    glGenTextures(1, &mTex);
    bind();
    GLenum format = 0;
    switch (fmt)
    {
        case InternalFormat::Color:
            mInternalFormat = GL_RGBA;
            format = GL_RGBA;
            break;
        case InternalFormat::Depth:
            mInternalFormat = GL_DEPTH_COMPONENT;
            format = GL_DEPTH_COMPONENT;
            break;
        case InternalFormat::Stencil:
            mInternalFormat = GL_STENCIL_INDEX;
            format =  GL_STENCIL_INDEX;
            break;
        case InternalFormat::DepthStencil:
            mInternalFormat = GL_DEPTH_STENCIL;
            format = GL_DEPTH_STENCIL;
            break;
    }
    glTexImage2D(mTarget, 0, mInternalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
}

void Texture::createMulti(GLsizei width, GLsizei height, InternalFormat fmt, std::uint8_t samples)
{
    free();
    mSize = glm::ivec2(width, height);
    mTarget = GL_TEXTURE_2D_MULTISAMPLE;
    mNumSamples = samples;

    glGenTextures(1, &mTex);
    bind();
    GLenum format = 0;
    switch (fmt)
    {
    case InternalFormat::Color:
        mInternalFormat = GL_RGBA;
        format = GL_RGBA;
        break;
    case InternalFormat::Depth:
        mInternalFormat = GL_DEPTH_COMPONENT;
        format = GL_DEPTH_COMPONENT;
        break;
    case InternalFormat::Stencil:
        mInternalFormat = GL_STENCIL_INDEX;
        format = GL_STENCIL_INDEX;
        break;
    case InternalFormat::DepthStencil:
        mInternalFormat = GL_DEPTH_STENCIL;
        format = GL_DEPTH_STENCIL;
        break;
    }
    glTexImage2DMultisample(mTarget, static_cast<GLsizei>(samples), mInternalFormat, width, height, GL_TRUE);
    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
}

void Texture::create(SoilImage const& image)
{
    free();
    mSize = glm::ivec2(image.width(), image.height());
    mTarget = GL_TEXTURE_2D;
    glGenTextures(1, &mTex);
    bind();
    mInternalFormat = GL_RGBA;
    glTexImage2D(mTarget, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
}

void Texture::createCubemap(SoilCubemapImage const& imgs)
{
    free();
    mSize = glm::ivec2(imgs[0].width(), imgs[0].height());
    mTarget = GL_TEXTURE_CUBE_MAP;
    glGenTextures(1, &mTex);
    bind();
    mInternalFormat = GL_RGBA;
    for (SoilCubemapImage::size_type i = 0; i < imgs.size(); ++i)
    {
        auto const& img = imgs[i];
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mInternalFormat, img.width(), img.height(),
            0, mInternalFormat, GL_UNSIGNED_BYTE, img.data());
    }
    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    unbind();
}

void Texture::setFilter(GLenum filter, GLint type)
{
    if (!mTex)
        return;
    bind();
    glTexParameteri(mTarget, filter, type);
    unbind();
}

void Texture::setWrap(GLenum axis, GLint type)
{
    if (!mTex)
        return;
    bind();
    glTexParameteri(mTarget, axis, type);
    unbind();
}

void Texture::genMipMap()
{
    if (!mTex)
        return;
    bind();
    glGenerateMipmap(mTarget);
    unbind();
}

void Texture::setType(TexType type)
{
    mType = type;
}

glm::ivec2 const& Texture::size() const
{
    return mSize;
}

Texture::operator GLuint() const
{
    return mTex;
}

GLint Texture::internalFormat() const
{
    return mInternalFormat;
}

bool Texture::isMultiSample() const
{
    return mTarget == GL_TEXTURE_2D_MULTISAMPLE;
}

std::uint8_t Texture::multiSamples() const
{
    return mNumSamples;
}

TexType Texture::type() const
{
    return mType;
}

void Texture::bind() const
{
    glBindTexture(mTarget, mTex);
}

void Texture::unbind() const
{
    glBindTexture(mTarget, 0);
}

void Texture::active(int textureSlotNum) const
{
    glActiveTexture(GL_TEXTURE0 + textureSlotNum);
    bind();
}

void Texture::active(Program const& prog, std::string const& name, int textureSlotNum) const
{
    glActiveTexture(GL_TEXTURE0 + textureSlotNum);
    bind();
    prog[name] = textureSlotNum;
}

void Texture::unactive(int textureSlotNum) const
{
    glActiveTexture(GL_TEXTURE0 + textureSlotNum);
    unbind();
}

void Texture::free()
{
    if (mTex)
    {
        mSize = glm::ivec2();
        glDeleteTextures(1, &mTex);
        mTex = 0;
        mTarget = 0;
        mInternalFormat = 0;
        mType = TexType::Normal;
        mNumSamples = 0;
    }
}
