#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"


Camera::Camera(float width, float height)
    : mSize(width, height)
    , mNear(0.1f)
    , mFar(7.0f)
    , mFov(45.0f)
    , mPos(0.0f, 0.0f, 5.0f)
    , mFront(0.0f, 0.0f, -1.0f)
    , mUp(0.0f, 1.0f, 0.0f)
{
}

Camera::Camera(int width, int height)
    : mSize(static_cast<float>(width), static_cast<float>(height))
    , mNear(0.1f)
    , mFar(7.0f)
    , mFov(45.0f)
    , mPos(0.0f, 0.0f, 5.0f)
    , mFront(0.0f, 0.0f, -1.0f)
    , mUp(0.0f, 1.0f, 0.0f)
{
}

Camera::Camera(size_t width, size_t height)
    : mSize(static_cast<float>(width), static_cast<float>(height))
    , mNear(0.1f)
    , mFar(7.0f)
    , mFov(45.0f)
    , mPos(0.0f, 0.0f, 5.0f)
    , mFront(0.0f, 0.0f, -1.0f)
    , mUp(0.0f, 1.0f, 0.0f)
{
}

float Camera::near() const
{
    return mNear;
}

float Camera::far() const
{
    return mFar;
}

float Camera::fov() const
{
    return mFov;
}

void Camera::near(float val)
{
    // TODO: maybe clamp by mNear?
    mNear = glm::clamp(val, 0.0f, std::numeric_limits<float>::max());
}

void Camera::far(float val)
{
    // TODO: maybe clamp by mFar?
    mFar = glm::clamp(val, 0.0f, std::numeric_limits<float>::max());
}

void Camera::fov(float val)
{
    mFov = glm::clamp(val, 0.0f, 360.0f);
}

glm::mat4 Camera::projection() const
{
    return glm::perspective(glm::radians(mFov), static_cast<GLfloat>(mSize.x) / static_cast<GLfloat>(mSize.y), mNear, mFar);
}

glm::vec3 const& Camera::pos() const
{
    return mPos;
}

glm::vec3 const& Camera::front() const
{
    return mFront;
}

glm::vec3 const& Camera::up() const
{
    return mUp;
}

void Camera::pos(glm::vec3 const& v)
{
    mPos = v;
}

void Camera::front(glm::vec3 const& v)
{
    mFront = glm::normalize(v);
}

void Camera::front(float pitch, float yaw)
{
    float const pitchRad = glm::radians(pitch);
    float const yawRad = glm::radians(yaw - 90);

    glm::vec3 front;
    front.x = std::cos(pitchRad) * std::cos(yawRad);
    front.y = std::sin(pitchRad);
    front.z = std::cos(pitchRad) * std::sin(yawRad);
    mFront = glm::normalize(front);
    glm::vec3 const right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), mFront));
    mUp = glm::normalize(glm::cross(mFront, right));
}

void Camera::up(glm::vec3 const& v)
{
    mUp = glm::normalize(v);
}

glm::mat4 Camera::view() const
{
    return glm::lookAt(mPos, mPos + mFront, mUp);
}

