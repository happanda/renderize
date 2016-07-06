#include "keyboard.h"


namespace
{
    void keyCallback(GLFWwindow*, int key, int scancode, int action, int modifiers)
    {
        KBRD().key(key, action, modifiers);
    }

    void charCallback(GLFWwindow* window, unsigned int symb)
    {
    }
} // anonymous namespace


Keyboard::Keyboard(GLFWwindow* window)
    : mWindow(window)
{
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetCharCallback(mWindow, charCallback);
}

void Keyboard::key(int key, int action, int modifiers)
{
    KeyAction ka = (action == GLFW_PRESS) ? KeyAction::Pressed
        : ((action == GLFW_RELEASE) ? KeyAction::Released : KeyAction::Repeated);
    int mods{ 0 };
    if (modifiers & GLFW_MOD_SHIFT)
        mods |= modShift;
    if (modifiers & GLFW_MOD_CONTROL)
        mods |= modControl;
    if (modifiers & GLFW_MOD_ALT)
        mods |= modAlt;
    if (modifiers & GLFW_MOD_SUPER)
        mods |= modSuper;
    sgnKey(key, ka, mods);
}

bool Keyboard::isPressed(int key)
{
    return glfwGetKey(mWindow, key) == GLFW_PRESS;
}
