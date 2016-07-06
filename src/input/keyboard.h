#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "input/input.h"
#include "util/signal.h"
#include "util/singleton.h"


struct GLFWwindow;


struct Keyboard
    : public Singleton<Keyboard>
{
    void key(int key, int action, int modifiers);
    bool isPressed(int key);

    signal<void(int, KeyAction, int)> sgnKey;

private:
    Keyboard(GLFWwindow* window);

    GLFWwindow* mWindow{ nullptr };

    FRIEND_SINGLETON(Keyboard)
};

INST_GET(Keyboard, KBRD)
