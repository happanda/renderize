#pragma once
#include <vector>
#include <glm/vec2.hpp>

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

    glm::ivec2  mWinSize;
    GLFWwindow* mWindow{ nullptr };
    std::vector<bool> mKeys;
    bool mMouseVisible{ false };
    glm::vec2 mMPos;
};


App& APP();
