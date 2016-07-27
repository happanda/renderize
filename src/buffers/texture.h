#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include "buffers/format.h"
#include "buffers/fwd.h"


enum class TexType
{
    Normal,
    Diffuse,
    Specular,
    Reflection
};

struct SoilImage;
typedef std::array<SoilImage, 6> SoilCubemapImage;
struct Program;

struct Texture
{
    Texture();
    ~Texture();
    Texture(Texture&& rhs);
    Texture const& operator=(Texture&& rhs);
    Texture(Texture const&) = delete;
    Texture const& operator=(Texture const&) = delete;

    void create(GLsizei width, GLsizei height, InternalFormat fmt);
    void createMulti(GLsizei width, GLsizei height, InternalFormat fmt, std::uint8_t samples);
    void create(SoilImage const& image);
    void createCubemap(SoilCubemapImage const& imgs);

    void setFilter(GLenum filter, GLint type);
    void setWrap(GLenum axis, GLint type);
    void genMipMap();

    void setType(TexType type);

    glm::ivec2 const& size() const;
    operator GLuint() const;
    GLint internalFormat() const;
    bool isMultiSample() const;
    std::uint8_t multiSamples() const;
    TexType type() const;

    void bind() const;
    void unbind() const;
    void active(int textureSlotNum) const;
    void active(Program const& prog, std::string const& name, int textureSlotNum) const;
    void unactive(int textureSlot) const;

    void free();

private:
    glm::ivec2 mSize;
    GLuint mTex{ 0 };
    GLenum mTarget{ 0 };
    GLint mInternalFormat{ 0 };
    TexType mType{ TexType::Normal };
    std::uint8_t mNumSamples; // should use glGetTexLevelParameter with TEXTURE_SAMPLES if possible
};
