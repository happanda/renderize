#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "buffers/texture.h"


struct Vertex
{
    glm::vec3  position;
    glm::vec3  normal;
    glm::vec2  texCoords;
};

struct Program;
struct Texture;


struct Mesh
{
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<TexturePtr> textures);
    ~Mesh();

    Mesh(Mesh&& rhs);
    Mesh const& operator=(Mesh&& rhs);
    Mesh(Mesh const&) = delete;
    Mesh const& operator=(Mesh const&) = delete;

    void noBlending();
    void blending(GLenum sfactor, GLenum dfactor);
    void blending(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

    void noCulling();
    void culling(GLenum mode);

    void draw(Program const& prog) const;

    void free();

    GLuint vao() const;
    GLuint vbo() const;
    GLuint ebo() const;

    std::vector<Vertex> const& vertices() const;
    std::vector<GLuint> const& indices() const;

private:
    void initMesh();

    GLuint mVAO{ 0 }, mVBO{ 0 }, mEBO{ 0 };

    std::vector<Vertex>  mVertices;
    std::vector<GLuint>  mIndices;
    std::vector<TexturePtr> mTextures;

    enum class Blending : char
    {
        None,
        Simple,
        Separate
    };
    Blending mBlending{ Blending::None };
    GLenum mSfactorRGB{ 0 };
    GLenum mDfactorRGB{ 0 };
    GLenum mSfactorAlpha{ 0 };
    GLenum mDfactorAlpha{ 0 };

    bool mCulling{ false };
    GLenum mCullMode{ 0 };
};
