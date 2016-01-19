#pragma once
#include <string>
#include <GL/glew.h>


struct Texture
{
    Texture();
    ~Texture();

    bool load(std::string const& path, bool genMipMap);
    void setFilter(GLenum filter, GLint type);
    void setWrap(GLenum axis, GLint type);

    operator GLuint() const;
    void bind() const;
    void unbind() const;
    void active(GLenum textureSlot) const;

private:
    GLuint mTex;
};
