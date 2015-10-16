#include "texture.h"
#include <SOIL.h>


texture::texture()
    : mTex(0)
{
}

texture::~texture()
{
    glDeleteTextures(1, &mTex);
}

bool texture::load(std::string const& path, bool genMipMap)
{
    glGenTextures(1, &mTex);
    bind();

    int texWidth = 0,
        texHeight = 0;
    auto image = SOIL_load_image("../tex/crate.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    if (!image)
    {
        glDeleteTextures(1, &mTex);
        return false;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (genMipMap)
        glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    unbind();
    return true;
}

void texture::setFilter(GLenum filter, GLint type)
{
    if (!mTex)
        return;
    bind();
    glTexParameteri(GL_TEXTURE_2D, filter, type);
    unbind();
}


void texture::setWrap(GLenum axis, GLint type)
{
    if (!mTex)
        return;
    bind();
    glTexParameteri(GL_TEXTURE_2D, axis, type);
    unbind();
}

texture::operator GLuint() const
{
    return mTex;
}

void texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, mTex);
}

void texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::active(GLenum textureSlot) const
{
    glActiveTexture(textureSlot);
    bind();
}
