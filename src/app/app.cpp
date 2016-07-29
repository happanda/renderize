#include <iostream>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "app.h"
#include "buffers/frame_buffer.h"
#include "buffers/render_buffer.h"
#include "buffers/render_target.h"
#include "buffers/texture.h"
#include "input/mouse.h"
#include "input/keyboard.h"
#include "shaders/program.h"
#include "shaders/program_manager.h"
#include "util/checked_call.h"
#include "util/date.h"

glm::mat3x3 sKernel;

using namespace std::placeholders;

namespace
{
    void glfwErrorReporting(int errCode, char const* msg)
    {
        APP().onGLFWError(errCode, msg);
    }

    void windowSizeCallback(GLFWwindow* window, int sizeX, int sizeY)
    {
        APP().resize(glm::ivec2(sizeX, sizeY));
    }
} // anonymous namespace


App::App()
    : mNumSamples(4)
    , mWinSize(800, 800)
    , mWindow(nullptr)
    , mScene(mWinSize)
    , mCamera(mWinSize)
    , mCameraBack(mWinSize)
{
}

App::~App()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

bool App::init()
{
    glfwSetErrorCallback(glfwErrorReporting);

    CHECK(GL_TRUE == glfwInit(), "GLFW init failed", return false);
    // Provide some hints for the GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a window
    //glfwWindowHint(GLFW_SAMPLES, 4);
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
    
    Mouse::create(mWindow);
    Keyboard::create(mWindow);
    KBRD().sgnKey.connect(std::bind(&App::onKey, this, _1, _2, _3));
    glfwSetWindowSizeCallback(mWindow, windowSizeCallback);

    mCamera.pos(glm::vec3(2.0f, 2.0f, 2.0f));
    mCamera.front(glm::vec3(0.0f, 0.0f, 0.0f));
    mScene.init();
    mCamUpdater.reset(new MainCameraUpdater(mCamera));
    mRTarget.reset(new RenderTarget(mWinSize, mNumSamples));
    mRTargetBack.reset(new RenderTarget(mWinSize, mNumSamples));
    return true;
}

bool App::shouldClose() const
{
    Keyboard::destroy();
    Mouse::destroy();
    return glfwWindowShouldClose(mWindow) != 0;
}

void App::resize(glm::ivec2 const& size)
{
    mWinSize = size;
    glfwMakeContextCurrent(mWindow);
    glViewport(0, 0, mWinSize.x, mWinSize.y);
    mScene.resize(mWinSize);
    mCamera.size(mWinSize);
    mCameraBack.size(mWinSize);
    mRTarget->size(mWinSize, mNumSamples);
    mRTargetBack->size(mWinSize, mNumSamples);
}

void App::onKey(int key, KeyAction action, int mods)
{
    if (action == KeyAction::Pressed)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(mWindow, GL_TRUE);
        else if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT)
        {
            MOUSE().visible(!MOUSE().visible());
        }
        else if (key == GLFW_KEY_F)
        {
            mSpotLightOn = !mSpotLightOn;
        }
    }
}

void App::onGLFWError(int errCode, char const* msg)
{
    std::cerr << "ERROR " << errCode << ": " << msg << std::endl;
}

void App::run()
{
    float lastTime = static_cast<float>(glfwGetTime());
    float dt{ 0.0f };
    float const dT{ 0.0125f };
    // Game Loop
    while (!glfwWindowShouldClose(mWindow))
    {
        glfwPollEvents();

        float const curTime = static_cast<float>(glfwGetTime());
        dt = curTime - lastTime;
        if (dt < dT)
            continue;
        lastTime = curTime;

        mScene.update(dt);
        mCamUpdater->update(dt);
        mCameraBack = mCamera;
        mCameraBack.front(-mCamera.front());

        // Rendering
        mRTarget->fb().bind();
        mScene.draw(mCamera);
        mRTarget->fb().unbind();

        mRTargetBack->fb().bind();
        mScene.draw(mCameraBack, glm::vec4(1.0f));
        mRTargetBack->fb().unbind();

        // Rendering
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        mRTarget->draw(glm::ivec2(0, 0), mWinSize);
        mRTargetBack->draw(glm::ivec2(0, 0), mWinSize / 8);

        glfwSwapBuffers(mWindow);
    }
}

void App::runFragmentDemo(std::string const& demoName)
{
    glm::vec3 quad[] = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f)
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_DYNAMIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);
    }


    ProgramManager progManager;
    Program prog = progManager.create("../shaders/fragment_demo.vert", "../shaders/" + demoName + ".frag");
    CHECK(prog, "Error creating shader program", return;);


    float lastTime = static_cast<float>(glfwGetTime());
    float dt{ 0.0f };
    float const dT{ 0.0125f };
    // Game Loop
    while (!glfwWindowShouldClose(mWindow))
    {
        glfwPollEvents();

        float const curTime = static_cast<float>(glfwGetTime());
        dt = curTime - lastTime;
        if (dt < dT)
            continue;
        lastTime = curTime;

        prog.use();

        glm::vec4 iDate(0.0f, 0.0f, 0.0f, secFrom00());
        prog["iResolution"] = glm::vec3(mWinSize, 0.0);
        prog["iGlobalTime"] = curTime;
        prog["iDate"] = iDate;

        // Rendering
        glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(mWindow);
    }
}
