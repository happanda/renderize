#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"


camera::camera(size_t width, size_t height)
    : mWidth(width)
    , mHeight(height)
    , mNear(0.1f)
    , mFar(100.0f)
    , mFov(45.0f)
    , mPos(0.0f, 0.0f, 3.0f)
    , mTarget(0.0f, 0.0f, 0.0f)
    , mUp(0.0f, 1.0f, 0.0f)
{
}

float camera::near() const
{
    return mNear;
}

float camera::far() const
{
    return mFar;
}

float camera::fov() const
{
    return mFov;
}

void camera::near(float val)
{
    // TODO: maybe clamp by mNear?
    mNear = glm::clamp(val, 0.0f, std::numeric_limits<float>::max());
}

void camera::far(float val)
{
    // TODO: maybe clamp by mFar?
    mFar = glm::clamp(val, 0.0f, std::numeric_limits<float>::max());
}

void camera::fov(float val)
{
    mFov = glm::clamp(val, 0.0f, 360.0f);
}

glm::mat4 camera::projection() const
{
    return glm::perspective(mFov, static_cast<GLfloat>(mWidth) / static_cast<GLfloat>(mHeight), mNear, mFar);
}

glm::vec3 const& camera::pos() const
{
    return mPos;
}

glm::vec3 const& camera::target() const
{
    return mTarget;
}

glm::vec3 const& camera::up() const
{
    return mUp;
}

void camera::pos(glm::vec3 const& v)
{
    mPos = v;
}

void camera::target(glm::vec3 const& v)
{
    mTarget = v;
}

void camera::up(glm::vec3 const& v)
{
    mUp = glm::normalize(v);
}

glm::mat4 camera::view() const
{
    return glm::lookAt(mPos, mTarget, mUp);
}

