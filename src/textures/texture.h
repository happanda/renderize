#pragma once
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

    bool load(std::string const& path, bool genMipMap);
    void setType(TexType type);
    void setFilter(GLenum filter, GLint type);
    void setWrap(GLenum axis, GLint type);

    operator GLuint() const;
    TexType type() const;
    void bind() const;
    void unbind() const;
    void active(GLenum textureSlot) const;

private:
    GLuint mTex;
    TexType mType;
};
