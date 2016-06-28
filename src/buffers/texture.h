#pragma once
#include <memory>
#include <string>
#include <GL/glew.h>

enum class TexType
{
    Normal,
    Diffuse,
    Specular
};

enum class TextureType
{
    Color,
    Depth,
    Stencil,
    DepthStencil
};

struct SoilImage;

struct Texture
{
    Texture();
    ~Texture();
    Texture(Texture&& rhs);
    Texture const& operator=(Texture&& rhs);
    Texture(Texture const&) = delete;
    Texture const& operator=(Texture const&) = delete;

    void create(GLsizei width, GLsizei height, TextureType ttype);
    void create(SoilImage const& image);

    void setFilter(GLenum filter, GLint type);
    void setWrap(GLenum axis, GLint type);
    void genMipMap();

    void setType(TexType type);

    operator GLuint() const;
    TexType type() const;


    void bind() const;
    void unbind() const;
    void active(GLenum textureSlot) const;
    void unactive(GLenum textureSlot) const;

private:
    GLuint mTex;
    TexType mType;
};

typedef std::shared_ptr<Texture>  TexturePtr;
