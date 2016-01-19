#include <cmath>
#include <glm/gtx/vector_query.hpp>
#include "point.h"


Point::Point(glm::vec3& pos)
    : mPnt(pos)
    , mTime(0.0f)
{
}

Point::~Point()
{
}

void Point::update(float dt)
{
    mTime += dt;
    updateImpl(dt);
}


PointCos::PointCos(glm::vec3& pos, glm::vec3 const& startPos, glm::vec3 const& finishPos)
    : Point(pos)
    , mPhase(0.0f)
    , mFreq(1.0f)
    , mStart(startPos)
    , mFinish(finishPos)
    , mDir(mFinish - mStart)
    , mDirLen(glm::length(mDir))
{
}

void PointCos::updateImpl(float)
{
    int const phase = static_cast<int>(mTime * mFreq / 3.14f) % 3;
    if (phase == 0)
        mPnt = mStart + mDir * (std::cos(mFreq * mTime + std::cos(mTime / 2.1f) * mPhase) + 1.0f) / 2.0f;
}


PointFromTo::PointFromTo(glm::vec3& pos, glm::vec3 const& finishPos)
    : Point(pos)
    , mMoveTime(2.0f)
    , mForceCoeff(2.4f)
    , mMass(1.0f)
    , mFrict(0.8f)
    , mFinish(finishPos)
    , mDir(mFinish - mPnt)
    , mDirLen(glm::length(mDir))
    , mForce(mDirLen * mForceCoeff)
{
}

float PointFromTo::dist() const
{
    return length(mDir);
}

void PointFromTo::updateImpl(float dt)
{
    mDir = mFinish - mPnt;
    mDirLen = length(mDir);
    if (mDirLen > 0.0f)
    {
        glm::vec3 const dirN = mDir / mDirLen;
        float const spNorm = length(mSpeed);
        glm::vec3 const speedUni = mSpeed / spNorm;

        mForce = dirN * mForceCoeff;
        if (spNorm > 0.0f)
            mForce -= speedUni * mFrict * mMass;
        glm::vec3 const accel = mForce / mMass;
        mSpeed += accel * dt;

        float const mSpLen = length(mSpeed);
        float const sMaxSpeed(2.0f);
        if (mSpLen > sMaxSpeed)
            mSpeed *= sMaxSpeed / mSpLen;
        //glm::vec3 const speedDir = dot(mSpeed, dirN) * dirN;
        //glm::vec3 const speedNorm = mSpeed - speedDir;
        //mSpeed = speedNorm + speedDir;
    }
    mPnt = mPnt + mSpeed * dt;
}
