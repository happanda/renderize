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

private:
    glm::ivec2  mWinSize;
    GLFWwindow* mWindow{ nullptr };
    std::vector<bool> mKeys;
    bool mMouseVisible{ false };
};
