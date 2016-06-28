#include "texture.h"
#include "util/soil_image.h"


Texture::Texture()
    : mTex(0)
    , mType(TexType::Normal)
{
}

Texture::~Texture()
{
    if (mTex)
        glDeleteTextures(1, &mTex);
}

Texture::Texture(Texture&& rhs)
    : mTex(rhs.mTex)
    , mType(rhs.mType)
{
    rhs.mTex = 0;
    rhs.mType = TexType::Normal;
}

Texture const& Texture::operator=(Texture&& rhs)
{
    mTex = rhs.mTex;
    mType = rhs.mType;
    rhs.mTex = 0;
    rhs.mType = TexType::Normal;
    return *this;
}

void Texture::create(GLsizei width, GLsizei height, TextureType ttype)
{
    glGenTextures(1, &mTex);
    bind();
    GLint internalFormat = 0;
    GLenum format = 0;
    switch (ttype)
    {
        case TextureType::Color:
            internalFormat = GL_RGBA;
            format = GL_RGBA;
            break;
        case TextureType::Depth:
            internalFormat = GL_DEPTH_COMPONENT;
            format = GL_DEPTH_COMPONENT;
            break;
        case TextureType::Stencil:
            internalFormat = GL_STENCIL_INDEX;
            format =  GL_STENCIL_INDEX;
            break;
        case TextureType::DepthStencil:
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
    bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
}
/*
bool Texture::load(std::string const& path, bool genMipMap)
{
    int texWidth = 0,
        texHeight = 0;
    auto image = SOIL_load_image(path.c_str(), &texWidth, &texHeight, 0, SOIL_LOAD_RGBA);
    if (!image)
    {
        glDeleteTextures(1, &mTex);
        return false;
    }
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    if (genMipMap)
        glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unbind();
    SOIL_free_image_data(image);
    return true;
}
*/
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
