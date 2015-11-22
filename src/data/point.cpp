#include <cmath>
#include <glm/gtx/vector_query.hpp>
#include "point.h"


point::point(glm::vec3& pos)
    : mPnt(pos)
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
    : point(pos)
    , mPhase(0.0f)
    , mFreq(1.0f)
    , mStart(startPos)
    , mFinish(finishPos)
    , mDir(mFinish - mStart)
    , mDirLen(glm::length(mDir))
{
}

void pointCos::updateImpl(float)
{
    int const phase = static_cast<int>(mTime * mFreq / 3.14f) % 3;
    if (phase == 0)
        mPnt = mStart + mDir * (std::cos(mFreq * mTime + std::cos(mTime / 2.1f) * mPhase) + 1.0f) / 2.0f;
}


pointFromTo::pointFromTo(glm::vec3& pos, glm::vec3 const& finishPos)
    : point(pos)
    , mMoveTime(2.0f)
    , mForceCoeff(1.0f)
    , mMass(1.0f)
    , mFrict(0.6f)
    , mFinish(finishPos)
    , mDir(mFinish - mPnt)
    , mDirLen(glm::length(mDir))
    , mForce(mDirLen * mForceCoeff)
{
}

float pointFromTo::dist() const
{
    return length(mDir);
}

void pointFromTo::updateImpl(float dt)
{
    mDir = mFinish - mPnt;
    mForce = mDir * mForceCoeff + (-mSpeed * mFrict);
    glm::vec3 const accel = mForce / mMass;
    mSpeed += accel * dt;
    mPnt = mPnt + mSpeed * dt;
}