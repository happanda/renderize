#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include "camera_updater.h"
#include "app/app.h"
#include "camera.h"
#include "input/mouse.h"
#include "input/keyboard.h"


MainCameraUpdater::MainCameraUpdater(Camera& camera)
    : mCamera(camera)
    , mMouseMoveConn(MOUSE().sgnMove.connect([this](glm::vec2 const& diff){ onMouseMove(diff); }))
    , mMouseScrollConn(MOUSE().sgnScroll.connect([this](glm::vec2 const& diff){ onMouseScroll(diff); }))
{
}

void MainCameraUpdater::update(float dt)
{
    float cameraSpeed = mCamera.speed() * dt;
    auto camPos = mCamera.pos();
    auto& kbd = KBRD();
    if (kbd.isPressed(GLFW_KEY_W))
        camPos += cameraSpeed * mCamera.front();
    if (kbd.isPressed(GLFW_KEY_S))
        camPos -= cameraSpeed * mCamera.front();
    if (kbd.isPressed(GLFW_KEY_A))
        camPos -= glm::normalize(glm::cross(mCamera.front(), mCamera.up())) * cameraSpeed;
    if (kbd.isPressed(GLFW_KEY_D))
        camPos += glm::normalize(glm::cross(mCamera.front(), mCamera.up())) * cameraSpeed;
    if (kbd.isPressed(GLFW_KEY_SPACE))
        camPos += glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * cameraSpeed;
    if (kbd.isPressed(GLFW_KEY_LEFT_SHIFT) || kbd.isPressed(GLFW_KEY_RIGHT_SHIFT))
        camPos -= glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * cameraSpeed;
    mCamera.pos(camPos);
    mCamera.front(mPitch, mYaw);
}

void MainCameraUpdater::onMouseMove(glm::vec2 const& diff)
{
    mYaw += diff.x;
    mPitch = glm::clamp(mPitch - diff.y, -89.0f, 89.0f);
}

void MainCameraUpdater::onMouseScroll(glm::vec2 const& diff)
{
    mCamera.fov(mCamera.fov() - static_cast<float>(diff.y));
}
