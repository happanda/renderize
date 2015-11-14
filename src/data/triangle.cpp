#include <iostream>
#include <glm/gtx/norm.hpp>
#include "triangle.h"
#include "cycle.h"


using namespace glm;

float const sMinFillDist{ 0.1f };

std::vector<vec3> fill(vec3 const* triangle, float minDist)
{
    std::vector<vec3> points;

    int idx = 0;
    vec3 edges[3];
    float lens[3];
    int nums[3];
    cycle2(triangle, triangle + 3, [&](vec3 const& p0, vec3 const& p1)
    {
        edges[idx] = p1 - p0;
        lens[idx] = length(edges[idx]);
        nums[idx] = static_cast<int>(lens[idx] / minDist);
        ++idx;
    });

    points.insert(points.cend(), triangle, triangle + 3);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 1; j < nums[i]; ++j)
        {
            float const step = static_cast<float>(j) / nums[i];
            points.push_back(triangle[i] + edges[i] * step);
        }
    }

    float const perimeter = lens[0] + lens[1] + lens[2];
    vec3 const inCenter = (lens[0] * triangle[2] + lens[1] * triangle[0] + lens[2] * triangle[1]) / perimeter;
    points.push_back(inCenter);

    return points;
}
