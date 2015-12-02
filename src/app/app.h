#pragma once
#include <vector>
#include <glm/vec2.hpp>

struct GLFWwindow;

struct app
{
    static void create();
    static void destroy();
    static app& inst();

    ~app();
    
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

    void runFragmentDemo(std::string const& demoName);

private:
    app();

    glm::ivec2  mWinSize;
    GLFWwindow* mWindow{ nullptr };
    std::vector<bool> mKeys;
    bool mMouseVisible{ false };
    glm::vec2 mMPos;
};


app& App();
