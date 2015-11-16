#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "app.h"
#include "util/checked_call.h"



void glfwErrorReporting(int errCode, char const* msg)
{
    std::cerr << "ERROR " << errCode << ": " << msg << std::endl;
}


app::app()
    : mWinSize(400, 400)
    , mWindow(nullptr)
    , mKeys(GLFW_KEY_LAST, false)
{
}

app::~app()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

bool app::init()
{
    auto glfwErrRep = [](int errCode, char const* msg)
    {
        std::cerr << "ERROR " << errCode << ": " << msg << std::endl;
    };
    glfwSetErrorCallback(glfwErrRep);

    CHECK(GL_TRUE == glfwInit(), "GLFW init failed", return false);
    // Provide some hints for the GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a window
    mWindow = glfwCreateWindow(mWinSize.x, mWinSize.y, "Renderize", nullptr, nullptr);
    
    CHECK(mWindow, "Error creating window", return false);
    glfwMakeContextCurrent(mWindow);

    // Initialize Glew
    glewExperimental = GL_TRUE;
    GLenum glewCode = glewInit();
    CHECK(GLEW_OK == glewCode, "Failed to initialize GLEW: ",
        std::cerr << glewGetErrorString(glewCode) << std::endl; return false);

    glViewport(0, 0, mWinSize.x, mWinSize.y);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    //glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_SRC_COLOR);

    // Initialize some GLFW callbacks
    //glfwSetInputMode(mWindow, GLFW_CURSOR, sMouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    //glfwSetKeyCallback(mWindow, keyCallback);
    //glfwSetCursorPosCallback(mWindow, mouseCallback);
    //glfwSetScrollCallback(mWindow, scrollCallback);
    //glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
    //glfwSetCharCallback(mWindow, charCallback);
    //glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
    return true;
}

bool app::shouldClose() const
{
    return glfwWindowShouldClose(mWindow) != 0;
}
