#include <utility>
#include "texture.h"
#include "util/soil_image.h"

using std::swap;


Texture::Texture()
    : mTex(0)
    , mInternalFormat(0)
    , mType(TexType::Normal)
{
}

Texture::~Texture()
{
    free();
}

Texture::Texture(Texture&& rhs)
    : mTex(0)
    , mInternalFormat(0)
    , mType(TexType::Normal)
{
    swap(mTex, rhs.mTex);
    swap(mInternalFormat, rhs.mInternalFormat);
    swap(mType, rhs.mType);
}

Texture const& Texture::operator=(Texture&& rhs)
{
    free();
    swap(mTex, rhs.mTex);
    swap(mType, rhs.mType);
    return *this;
}

void Texture::create(GLsizei width, GLsizei height, InternalFormat fmt)
{
    free();

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
    glTexImage2D(GL_TEXTURE_2D, 0, mInternalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
}

void Texture::create(SoilImage const& image)
{
    glGenTextures(1, &mTex);
    bind();
    mInternalFormat = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    unbind();
}

void Texture::setFilter(GLenum filter, GLint type)
{
    if (!mTex)
        return;
    bind();
    glTexParameteri(GL_TEXTURE_2D, filter, type);
    unbind();
}

void Texture::setWrap(GLenum axis, GLint type)
{
    if (!mTex)
        return;
    bind();
    glTexParameteri(GL_TEXTURE_2D, axis, type);
    unbind();
}

void Texture::genMipMap()
{
    if (!mTex)
        return;
    bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}

void Texture::setType(TexType type)
{
    mType = type;
}

Texture::operator GLuint() const
{
    return mTex;
}

GLint Texture::internalFormat() const
{
    return mInternalFormat;
}

TexType Texture::type() const
{
    return mType;
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, mTex);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::active(GLenum textureSlot) const
{
    glActiveTexture(textureSlot);
    bind();
}

void Texture::unactive(GLenum textureSlot) const
{
    glActiveTexture(textureSlot);
    unbind();
}

void Texture::free()
{
    if (mTex)
    {
        glDeleteTextures(1, &mTex);
        mTex = 0;
        mInternalFormat = 0;
        mType = TexType::Normal;
    }
}
