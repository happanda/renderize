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
    : mWindow(window)
    , mSensitivity(0.08f)
    , mVisible(true)
{
    glfwSetInputMode(mWindow, GLFW_CURSOR, mVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, mouseCallback);
    glfwSetScrollCallback(mWindow, scrollCallback);
    glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
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
    sgnMove(diff);
    mPos = vec;
}

glm::vec2 const& Mouse::pos() const
{
    return mPos;
}

void Mouse::scroll(glm::vec2 const& diff)
{
    sgnScroll(diff);
}

void Mouse::button(int button, int action, int /*modifiers*/)
{
    MouseButton mb;
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        mb = MouseButton::Left;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        mb = MouseButton::Right;
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        mb = MouseButton::Middle;
    else
        return;
    KeyAction ka = (action == GLFW_PRESS) ? KeyAction::Pressed : KeyAction::Released;
    sgnButton(mPos, mb, ka);
}

void Mouse::visible(bool vis)
{
    mVisible = vis;
    glfwSetInputMode(mWindow, GLFW_CURSOR, mVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

bool Mouse::visible() const
{
    return mVisible;
}