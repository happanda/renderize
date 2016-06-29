#include <utility>
#include "texture.h"
#include "util/soil_image.h"

using std::swap;


Texture::Texture()
    : mTex(0)
    , mType(TexType::Normal)
{
}

Texture::~Texture()
{
    free();
}

Texture::Texture(Texture&& rhs)
    : mTex(0)
    , mType(TexType::Normal)
{
    swap(mTex, rhs.mTex);
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
    GLint internalFormat = 0;
    GLenum format = 0;
    switch (fmt)
    {
        case InternalFormat::Color:
            internalFormat = GL_RGBA;
            format = GL_RGBA;
            break;
        case InternalFormat::Depth:
            internalFormat = GL_DEPTH_COMPONENT;
            format = GL_DEPTH_COMPONENT;
            break;
        case InternalFormat::Stencil:
            internalFormat = GL_STENCIL_INDEX;
            format =  GL_STENCIL_INDEX;
            break;
        case InternalFormat::DepthStencil:
            internalFormat = GL_DEPTH_STENCIL;
            format = GL_DEPTH_STENCIL;
            break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
    unbind();
}

void Texture::create(SoilImage const& image)
{
    glGenTextures(1, &mTex);
    bind();
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
        mType = TexType::Normal;
    }
}
