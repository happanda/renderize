#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include "camera_updater.h"
#include "app/app.h"
#include "camera.h"

MainCameraUpdater::MainCameraUpdater(Camera& camera)
    : mCamera(camera)
{
}

// TODO: on mouse events
// on moving:
// mYaw += xDiff;
// mPitch = glm::clamp(mPitch - yDiff, -89.0f, 89.0f);

// on scrolling:
// mCamera.fov(mCamera.fov() - static_cast<float>(yDiff));


void MainCameraUpdater::update(float dt)
{
    float cameraSpeed = mCamera.speed() * dt;
    auto camPos = mCamera.pos();
    auto& app = APP();
    if (app.isPressed(GLFW_KEY_W))
        camPos += cameraSpeed * mCamera.front();
    if (app.isPressed(GLFW_KEY_S))
        camPos -= cameraSpeed * mCamera.front();
    if (app.isPressed(GLFW_KEY_A))
        camPos -= glm::normalize(glm::cross(mCamera.front(), mCamera.up())) * cameraSpeed;
    if (app.isPressed(GLFW_KEY_D))
        camPos += glm::normalize(glm::cross(mCamera.front(), mCamera.up())) * cameraSpeed;
    if (app.isPressed(GLFW_KEY_SPACE))
        camPos += glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * cameraSpeed;
    if (app.isPressed(GLFW_KEY_LEFT_SHIFT) || app.isPressed(GLFW_KEY_RIGHT_SHIFT))
        camPos -= glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * cameraSpeed;
    mCamera.pos(camPos);
    mCamera.front(mPitch, mYaw);
}
