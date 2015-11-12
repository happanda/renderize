#pragma once
#include <vector>
#include <glm/vec3.hpp>


extern float const sMinFillDist;

std::vector<glm::vec3> fill(glm::vec3 const* triangle, float minDist = sMinFillDist);
