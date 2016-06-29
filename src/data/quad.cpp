#include "quad.h"

std::array<GLfloat, sNumQuadVertices> sQuadVertices = {
    // Positions           // Normals           // Texture Coords
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f
};

Mesh quadMesh(std::vector<TexturePtr> const& textures)
{
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < sNumQuadVertices; i += 8)
    {
        Vertex vertex = {
            { sQuadVertices[i], sQuadVertices[i + 1], sQuadVertices[i + 2] },
            { sQuadVertices[i + 3], sQuadVertices[i + 4], sQuadVertices[i + 5] },
            { sQuadVertices[i + 6], sQuadVertices[i + 7] }
        };
        vertices.emplace_back(vertex);
    }
    return Mesh(vertices, std::vector<GLuint>(), textures);
}

std::array<GLfloat, sNumScreenQuadVertices> sScreenQuadVertices = {
    // Positions  // TexCoords
    -1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f
};
