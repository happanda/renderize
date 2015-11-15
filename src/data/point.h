#pragma once
#include <glm/vec3.hpp>


struct point
{
    point(glm::vec3& pos);
    virtual ~point();

    void update(float dt);

protected:
    virtual void updateImpl(float dt) = 0;

    glm::vec3&  mPnt;
    float  mTime;
};

struct pointCos
    : public point
{
    pointCos(glm::vec3& pos, glm::vec3 const& startPos, glm::vec3 const& finishPos);

    float mPhase;
    float mFreq;

private:
    void updateImpl(float dt) override;

    glm::vec3  mStart;
    glm::vec3  mFinish;
    glm::vec3  mDir;
    float  mDirLen;
};


template <class Iter>
struct pointCycle
    : public point
{
    pointCycle(glm::vec3& pos, Iter first, Iter last, Iter current)
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
