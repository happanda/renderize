#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include "camera/camera.h"

struct GLFWwindow;

struct App
{
    static void create();
    static void destroy();
    static App& inst();

    ~App();
    
    bool init();
    bool shouldClose() const;
    
    void resize(int width, int height);
    
    void mouse(glm::vec2 const& pos);
    void scroll(float xDiff, float yDiff);
    void touchDown(int button);
    void touchUp(int button);
    void keyDown(int key);
    void keyUp(int key);
    bool isPressed(int key);
    void onGLFWError(int errCode, char const* msg);

    void run();
    void runFragmentDemo(std::string const& demoName);

private:
    App();
    void moveCamera(float dt);

    glm::ivec2  mWinSize;
    GLFWwindow* mWindow{ nullptr };
    std::vector<bool> mKeys;
    bool mMouseVisible{ true };
    bool mSpotLightOn{ false };
    glm::vec2 mMPos;

    float mYaw{ 0.0f };
    float mPitch{ 0.0f };
    Camera mCamera;
};


App& APP();
