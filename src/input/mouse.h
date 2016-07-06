#pragma once
#include <glm/vec2.hpp>
#include "input/input.h"
#include "util/signal.h"
#include "util/singleton.h"


struct GLFWwindow;

enum class MouseButton
{
    Left,
    Right,
    Middle
};

struct Mouse
    : public Singleton<Mouse>
{
    void sensitivity(float sens);
    float sensitivity() const;

    void pos(glm::vec2 const& vec);
    glm::vec2 const& pos() const;

    void scroll(glm::vec2 const& diff);

    void button(int button, int action, int modifiers);

    void visible(bool vis);
    bool visible() const;

    signal<void(glm::vec2 const&)> sgnMove;
    signal<void(glm::vec2 const&)> sgnScroll;
    signal<void(glm::vec2 const&, MouseButton, KeyAction)> sgnButton;

private:
    Mouse(GLFWwindow* window);
    FRIEND_SINGLETON(Mouse)

    float mSensitivity;
    glm::vec2 mPos;

    bool mVisible;
};

INST_GET(Mouse, MOUSE)
