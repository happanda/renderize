#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "textures/texture.h"


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
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<TexturePtr> textures);
    ~Mesh();

    void noBlending();
    void blending(GLenum sfactor, GLenum dfactor);
    void blending(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

    void noCulling();
    void culling(GLenum mode);

    void draw(Program const& prog) const;
    
    Mesh(Mesh&& rhs);
    Mesh const& operator=(Mesh&& rhs);

private:
    void initMesh();

    GLuint mVAO, mVBO, mEBO;

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
