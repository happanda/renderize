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
    void draw(Program const& prog) const;

    Mesh(Mesh&& rhs);
    Mesh const& operator=(Mesh&& rhs);

private:
    void initMesh();

    GLuint mVAO, mVBO, mEBO;

    std::vector<Vertex>  mVertices;
    std::vector<GLuint>  mIndices;
    std::vector<TexturePtr> mTextures;
};
