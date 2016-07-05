#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include "camera/camera.h"
#include "util/singleton.h"


struct GLFWwindow;

struct App
    : public Singleton<App>
{
    ~App();

    bool init();
    bool shouldClose() const;

    void resize(glm::ivec2 const& size);

    void keyDown(int key);
    void keyUp(int key);
    bool isPressed(int key);
    void onGLFWError(int errCode, char const* msg);

    void run();
    void runFragmentDemo(std::string const& demoName);

private:
    App();
    FRIEND_SINGLETON(App);

    glm::ivec2  mWinSize;
    GLFWwindow* mWindow{ nullptr };
    std::vector<bool> mKeys;
    bool mMouseVisible{ true };
    bool mSpotLightOn{ false };
    glm::vec2 mMPos;
};


INST_GET(App, APP);
