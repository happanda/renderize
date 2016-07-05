#include <GLFW/glfw3.h>
#include "mouse.h"


namespace
{
    void mouseCallback(GLFWwindow*, double x, double y)
    {
        MOUSE().pos(glm::vec2(static_cast<float>(x), static_cast<float>(y)));
    }

    void scrollCallback(GLFWwindow*, double xDiff, double yDiff)
    {
        MOUSE().scroll(glm::vec2(static_cast<float>(xDiff), static_cast<float>(yDiff)));
    }

    void mouseButtonCallback(GLFWwindow*, int button, int action, int modifiers)
    {
        MOUSE().button(button, action, modifiers);
    }
} // anonymous namespace


Mouse::Mouse(GLFWwindow* window)
    : mSensitivity(0.08f)
    , mVisible(true)
{
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void Mouse::sensitivity(float sens)
{
    mSensitivity = sens;
}

float Mouse::sensitivity() const
{
    return mSensitivity;
}

void Mouse::pos(glm::vec2 const& vec)
{
    static glm::vec2 lastPos = vec;

    if (mVisible)
    {
        lastPos = vec;
        return;
    }

    glm::vec2 diff = vec - lastPos;
    lastPos = vec;
    diff *= mSensitivity;
    // TODO: signal diff
    mPos = vec;
}

glm::vec2 const& Mouse::pos() const
{
    return mPos;
}

void Mouse::scroll(glm::vec2 const& diff)
{
    // TODO: signal scroll
}

void Mouse::button(int button, int action, int modifiers)
{
    // TODO: signal button
}

void Mouse::visible(bool vis)
{
    mVisible = vis;
}

bool Mouse::visible() const
{
    return mVisible;
}