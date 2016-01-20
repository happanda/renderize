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
    Mesh(std::vector<Vertex> vertices, std::vector<GLint> indices, std::vector<Texture> textures);
    void draw(Program const& prog);

    std::vector<Vertex>  vertices;
    std::vector<GLint>   indices;
    std::vector<Texture> textures;

private:
    GLuint mVAO, mVBO, mEBO;

    void initMesh();
};
