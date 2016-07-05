#pragma once


struct Camera;

struct MainCameraUpdater
{
    MainCameraUpdater(Camera& camera);

    void update(float dt);

private:
    Camera& mCamera;
    float mYaw{ 0.0f };
    float mPitch{ 0.0f };
};
