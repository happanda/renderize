#pragma once
#include <glm/vec3.hpp>


struct Point
{
    Point(glm::vec3& pos);
    Point& operator=(Point const& other) = default;
    virtual ~Point();

    void update(float dt);

protected:
    virtual void updateImpl(float dt) = 0;

    glm::vec3&  mPnt;
    float  mTime;
};

struct PointCos
    : public Point
{
    PointCos(glm::vec3& pos, glm::vec3 const& startPos, glm::vec3 const& finishPos);
    PointCos& operator=(PointCos const& other) = default;

    float mPhase;
    float mFreq;

private:
    void updateImpl(float dt) override;

    glm::vec3  mStart;
    glm::vec3  mFinish;
    glm::vec3  mDir;
    float  mDirLen;
};


struct PointFromTo
    : public Point
{
    PointFromTo(glm::vec3& pos, glm::vec3 const& finishPos);
    PointFromTo& operator=(PointFromTo const& other) = default;

    float mMoveTime;
    float mForceCoeff;
    float mMass;
    float mFrict;

    float dist() const;

    glm::vec3  mFinish;
    glm::vec3  mSpeed;

private:
    void updateImpl(float dt) override;

    glm::vec3  mDir;
    float  mDirLen;
    glm::vec3  mForce;
};


template <class Iter>
struct PointCycle
    : public Point
{
    PointCycle(glm::vec3& pos, Iter first, Iter last, Iter current)
        : point(pos)
        , mFirst(first)
        , mLast(last)
        , mNext(current)
        , mPhase(0.0f)
        , mFreq(1.0f)
    {
        getNextPos();
    }

    float mPhase;
    float mFreq;

private:
    void updateImpl(float dt) override
    {
        int const phase = static_cast<int>(mTime * mFreq);
        if (phase % 2 == 1)
        {
            float const locTime = mTime * mFreq - phase;
            mPnt = mCurPos + mDir * std::sin(locTime * 3.14f / 2.0f);
            mFoundNext = false;
        }
        else if (!mFoundNext)
        {
            getNextPos();
            mFoundNext = true;
        }
    }

    void getNextPos()
    {
        mCurPos = mPnt;
        ++mNext;
        if (mNext == mLast)
            mNext = mFirst;
        mNextPos = *mNext;
        mDir = mNextPos - mCurPos;
    }

    Iter mFirst;
    Iter mLast;
    Iter mNext;
    glm::vec3 mNextPos;
    glm::vec3 mCurPos;
    glm::vec3 mDir;
    bool mFoundNext;
};
