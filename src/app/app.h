#pragma once
#include <vector>
#include <glm/vec2.hpp>

struct GLFWwindow;

struct app
{
    app();
    ~app();
    
    bool init();
    bool shouldClose() const;
    
    void resize(int width, int height);
    
    void mouse(glm::vec2 const& pos);
    void scroll(int xDiff, int yDiff);
    void touchDown(int button);
    void touchUp(int button);
    void keyDown(int key);
    void keyUp(int key);
    
    bool isPressed(int key);
    
private:
    glm::ivec2  mWinSize;
    GLFWwindow* mWindow{ nullptr };
    std::vector<bool> mKeys;
    bool mMouseVisible{ false };
    glm::vec2 mMPos;
};
