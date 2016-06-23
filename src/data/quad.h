#pragma once
#include <array>
#include <vector>
#include <GL/glew.h>
#include "data/mesh.h"


const int sNumQuadVertices = (3 + 3 + 2) * 6;
extern std::array<GLfloat, sNumQuadVertices>  sQuadVertices;

Mesh quadMesh(std::vector<TexturePtr> const& textures);
