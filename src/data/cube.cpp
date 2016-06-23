#include "cube.h"
#include <glm/gtx/norm.hpp>


std::array<GLfloat, sNumVertices> sVertices = {
    // Positions           // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

std::vector<glm::vec3> getCube(float sideLen, size_t vertsOnSide)
{
    float const hs = sideLen / 2.0f;
    float const dx = sideLen / (vertsOnSide + 1);
    std::vector<glm::vec3> cube;
    cube.push_back(glm::vec3(-hs, -hs, -hs));
    cube.push_back(glm::vec3(-hs, -hs,  hs));
    cube.push_back(glm::vec3(-hs,  hs,  hs));
    cube.push_back(glm::vec3(-hs,  hs, -hs));
    cube.push_back(glm::vec3( hs, -hs, -hs));
    cube.push_back(glm::vec3( hs, -hs,  hs));
    cube.push_back(glm::vec3( hs,  hs,  hs));
    cube.push_back(glm::vec3( hs,  hs, -hs));

    glm::vec3 offset;
    float* axis[3] = { &offset.x, &offset.y, &offset.z };
    
    for (size_t it = 0; it < 3; ++it)
    {
        for (int sign = -1; sign <= 1; sign += 2)
        {
            *axis[0] = sign * hs;

            for (size_t x = 0; x <= vertsOnSide + 1; ++x)
            {
                for (size_t y = 0; y <= vertsOnSide + 1; ++y)
                {
                    *axis[1] = -hs + x * dx;
                    *axis[2] = -hs + y * dx;

                    cube.push_back(offset);
                }
            }
        }

        auto axis0 = axis[0];
        axis[0] = axis[1];
        axis[1] = axis[2];
        axis[2] = axis0;
    }

    return cube;
}

Mesh cubeMesh(std::vector<TexturePtr> const& textures)
{
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < sNumVertices; i += 8)
    {
        Vertex vertex = {
            { sVertices[i], sVertices[i + 1], sVertices[i + 2] },
            { sVertices[i + 3], sVertices[i + 4], sVertices[i + 5] },
            { sVertices[i + 6], sVertices[i + 7] }
        };
        vertices.emplace_back(vertex);
    }
    return Mesh(vertices, std::vector<GLuint>(), textures);
}