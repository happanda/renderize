#include "texture.h"
#include <SOIL.h>


Texture::Texture()
    : mTex(0)
    , mType(TexType::Normal)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &mTex);
}

bool Texture::load(std::string const& path, bool genMipMap)
{
    glGenTextures(1, &mTex);
    bind();

    int texWidth = 0,
        texHeight = 0;
    auto image = SOIL_load_image(path.c_str(), &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    if (!image)
    {
        glDeleteTextures(1, &mTex);
        return false;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (genMipMap)
        glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    unbind();
    return true;
}

void Texture::setType(TexType type)
{
    mType = type;
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
