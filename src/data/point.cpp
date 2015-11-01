#include <cmath>
#include <glm/gtx/vector_query.hpp>
#include "point.h"


point::point(glm::vec3& pos, glm::vec3 const& startPos, glm::vec3 const& finishPos)
    : mPnt(pos)
    , mStart(startPos)
    , mFinish(finishPos)
    , mDir(mFinish - mStart)
    , mTime(0.0f)
{
}

point::~point()
{
}

void point::update(float dt)
{
    mTime += dt;
    updateImpl(dt);
}


pointCos::pointCos(glm::vec3& pos, glm::vec3 const& startPos, glm::vec3 const& finishPos)
    : point(pos, startPos, finishPos)
    , mPhase(0.0f)
    , mFreq(1.0f)
    , mDirLen(glm::length(mDir))
{
}

void pointCos::updateImpl(float)
{
    int const phase = int(mTime * mFreq / 3.14f) % 3;
    if (phase == 0)
        mPnt = mStart + mDir * (std::cos(mFreq * mTime + std::cos(mTime / 2.1f) * mPhase) + 1.0f) / 2.0f;
}
