#include <algorithm>
#include <glm/gtx/norm.hpp>
#include "triangle.h"
#include "cycle.h"


using namespace glm;

float const sMinFillDist{ 0.1f };

void fill(vec3 const* triangle, std::vector<vec3>& points, float minDist)
{
    points.insert(points.cend(), triangle, triangle + 3);

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

    idx = 0;
    vec3 toCent[3];
    int minSteps = -1;
    bool goToCenter = true;
    std::for_each(triangle, triangle + 3, [&](vec3 const& p)
    {
        toCent[idx] = inCenter - p;
        float const toC = length(inCenter - p);
        int steps = static_cast<int>(toC / minDist);
        if (steps > 1)
        {
            if (minSteps < 0 || minSteps > steps)
                minSteps = steps;
        }
        else
            goToCenter = false;
        ++idx;
    });

    if (goToCenter)
    {
        vec3 newTriangle[3];
        idx = 0;
        std::transform(triangle, triangle + 3, newTriangle, [&](vec3 const& p)
        {
            return p + toCent[idx++] * (1.0f / static_cast<float>(minSteps));
        });
        fill(newTriangle, points, minDist);
    }
}

std::vector<vec3> fill(vec3 const* triangle, float minDist)
{
    std::vector<vec3> points;
    fill(triangle, points, minDist);

    return points;
}
