#pragma once
#include <array>
#include <memory>
#include <string>
#include <GL/glew.h>
#include "buffers/format.h"
#include "buffers/fwd.h"


enum class TexType
{
    Normal,
    Diffuse,
    Specular
};

struct SoilImage;
typedef std::array<SoilImage, 6> SoilCubemapImage;

struct Texture
{
    Texture();
    ~Texture();
    Texture(Texture&& rhs);
    Texture const& operator=(Texture&& rhs);
    Texture(Texture const&) = delete;
    Texture const& operator=(Texture const&) = delete;

    void create(GLsizei width, GLsizei height, InternalFormat fmt);
    void create(SoilImage const& image);
    void createCubemap(SoilCubemapImage const& imgs);

    void setFilter(GLenum filter, GLint type);
    void setWrap(GLenum axis, GLint type);
    void genMipMap();

    void setType(TexType type);

    operator GLuint() const;
    GLint internalFormat() const;
    TexType type() const;

    void bind() const;
    void unbind() const;
    void active(GLenum textureSlot) const;
    void unactive(GLenum textureSlot) const;

    void free();

private:
    GLuint mTex{ 0 };
    GLenum mTarget{ 0 };
    GLint mInternalFormat{ 0 };
    TexType mType{ TexType::Normal };
};
