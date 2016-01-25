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

struct Texture
{
    Texture();
    ~Texture();

    bool load(std::string const& path, bool genMipMap = true);
    void setType(TexType type);
    void setFilter(GLenum filter, GLint type);
    void setWrap(GLenum axis, GLint type);

    operator GLuint() const;
    TexType type() const;
    void bind() const;
    void unbind() const;
    void active(GLenum textureSlot) const;
    void unactive(GLenum textureSlot) const;

private:
    Texture(Texture const&);
    Texture const& operator=(Texture const&);

    GLuint mTex;
    TexType mType;
};

typedef std::shared_ptr<Texture>  TexturePtr;
