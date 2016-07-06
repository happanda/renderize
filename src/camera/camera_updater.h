#pragma once
#include "util/signal.h"


struct Camera;

struct MainCameraUpdater
{
    MainCameraUpdater(Camera& camera);

    void update(float dt);

private:
    void onMouseMove(glm::vec2 const& diff);
    void onMouseScroll(glm::vec2 const& diff);

    Camera& mCamera;
    float mYaw{ 0.0f };
    float mPitch{ 0.0f };

    connection mMouseMoveConn;
    connection mMouseScrollConn;
};
