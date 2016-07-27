#pragma once
#include <cstdint>
#include <glm/vec2.hpp>
#include "app/scene.h"
#include "camera/camera.h"
#include "camera/camera_updater.h"
#include "input/input.h"
#include "util/singleton.h"


struct GLFWwindow;

struct App
    : public Singleton<App>
{
    ~App();

    bool init();
    bool shouldClose() const;

    void resize(glm::ivec2 const& size);
    void onKey(int key, KeyAction action, int mods);

    void onGLFWError(int errCode, char const* msg);

    void run();
    void runFragmentDemo(std::string const& demoName);

private:
    App();
    FRIEND_SINGLETON(App);

    std::uint8_t mNumSamples; // multisampling parameter
    glm::ivec2  mWinSize;
    GLFWwindow* mWindow{ nullptr };
    bool mSpotLightOn{ false };
        
    Scene mScene;
    Camera mCamera;
    Camera mCameraBack;
    std::shared_ptr<MainCameraUpdater> mCamUpdater;
    RenderTargetPtr mRTarget;
    RenderTargetPtr mRTargetBack;
};


INST_GET(App, APP);
