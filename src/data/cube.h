#pragma once
#include <array>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include "data/mesh.h"


const int sNumVertices = (3 + 3 + 2) * 6 * 6;
extern std::array<GLfloat, sNumVertices>  sVertices;

std::vector<glm::vec3> getCube(float sideLen, size_t vertsOnSide);

Mesh cubeMesh(std::vector<TexturePtr> const& textures);
