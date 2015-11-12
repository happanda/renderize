#include <glm/gtx/norm.hpp>
#include "triangle.h"


using namespace glm;

float const sMinFillDist{ 0.1f };

std::vector<vec3> fill(vec3 const* triangle, float minDist)
{
    std::vector<vec3> points;

    vec3 const& A = triangle[0];
    vec3 const& B = triangle[1];
    vec3 const& C = triangle[2];
    points.push_back(A);
    points.push_back(B);
    points.push_back(C);

    vec3 const AB = B - A;
    vec3 const BC = C - B;
    vec3 const CA = A - C;
    float const lenAB = length(AB);
    float const lenBC = length(BC);
    float const lenCA = length(CA);

    int const numAB = static_cast<int>(lenAB / minDist);
    int const numBC = static_cast<int>(lenBC / minDist);
    int const numCA = static_cast<int>(lenCA / minDist);
    
    for (int i = 1; i < numAB; ++i)
    {
        float const step = static_cast<float>(i) / numAB;
        points.push_back(A + AB * step);
    }
    for (int i = 1; i < numBC; ++i)
    {
        float const step = static_cast<float>(i) / numBC;
        points.push_back(B + BC * step);
    }
    for (int i = 1; i < numCA; ++i)
    {
        float const step = static_cast<float>(i) / numCA;
        points.push_back(C + CA * step);
    }
    //points.push_back(C + CA * 0.5f);

    float const perimeter = lenAB + lenBC + lenCA;
    vec3 const inCenter = (lenAB * C + lenBC * A + lenCA * B) / perimeter;
    points.push_back(inCenter);

    return points;
}
