#pragma once
#include <glm/vec3.hpp>


struct point
{
    point(glm::vec3& pos, glm::vec3 const& startPos, glm::vec3 const& finishPos);
    virtual ~point();

    void update(float dt);

protected:
    virtual void updateImpl(float dt) = 0;

    glm::vec3&  mPnt;
    glm::vec3  mStart;
    glm::vec3  mFinish;
    glm::vec3  mDir;
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

    float  mDirLen;
};
