#include <iostream>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include "app.h"
#include "shaders/program.h"
#include "util/checked_call.h"
#include "util/date.h"


namespace
{
    app*  sAppInstance{ nullptr };

    void glfwErrorReporting(int errCode, char const* msg)
    {
        App().onGLFWError(errCode, msg);
    }

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
    {
        if (action == GLFW_PRESS)
            App().keyDown(key);
        else
            App().keyUp(key);
    }

    void mouseCallback(GLFWwindow* window, double x, double y)
    {
        App().mouse(glm::vec2(static_cast<float>(x), static_cast<float>(y)));
    }

    void scrollCallback(GLFWwindow* window, double xDiff, double yDiff)
    {
        App().scroll(static_cast<float>(xDiff), static_cast<float>(yDiff));
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers)
    {
        App().touchDown(button);
    }

    void charCallback(GLFWwindow* window, unsigned int symb)
    {
    }

    void windowSizeCallback(GLFWwindow* window, int sizeX, int sizeY)
    {
        App().resize(sizeX, sizeY);
    }
} // anonymous namespace



void app::create()
{
    if (!sAppInstance)
        sAppInstance = new app();
}

void app::destroy()
{
    if (sAppInstance)
    {
        delete sAppInstance;
        sAppInstance = nullptr;
    }
}

app& app::inst()
{
    return *sAppInstance;
}

app::app()
    : mWinSize(800, 800)
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
    glfwSetErrorCallback(glfwErrorReporting);

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
    glfwSetInputMode(mWindow, GLFW_CURSOR, mMouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetCursorPosCallback(mWindow, mouseCallback);
    glfwSetScrollCallback(mWindow, scrollCallback);
    glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
    glfwSetCharCallback(mWindow, charCallback);
    glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
    return true;
}

bool app::shouldClose() const
{
    return glfwWindowShouldClose(mWindow) != 0;
}

void app::resize(int width, int height)
{
    mWinSize = glm::ivec2(width, height);
    glViewport(0, 0, mWinSize.x, mWinSize.y);
}

void app::mouse(glm::vec2 const& pos)
{
    float const xFloat = static_cast<GLfloat>(pos.x);
    float const yFloat = static_cast<GLfloat>(pos.y);
    static GLfloat lastX = xFloat, lastY = yFloat;

    if (mMouseVisible)
    {
        lastX = xFloat;
        lastY = yFloat;
        return;
    }

    GLfloat xDiff = xFloat - lastX;
    GLfloat yDiff = yFloat - lastY;
    lastX = xFloat;
    lastY = yFloat;
    GLfloat const sensitivity = 0.08f;
    xDiff *= sensitivity;
    yDiff *= sensitivity;
}

void app::scroll(float xDiff, float yDiff)
{
    //sCamera.fov(sCamera.fov() - static_cast<float>(yDiff));
}

void app::touchDown(int button)
{
}

void app::touchUp(int button)
{
}

void app::keyDown(int key)
{
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(mWindow, GL_TRUE);
    else if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT)
    {
        mMouseVisible = !mMouseVisible;
        glfwSetInputMode(mWindow, GLFW_CURSOR, mMouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}

void app::keyUp(int key)
{
}

bool app::isPressed(int key)
{
    return mKeys[key];
}

void app::onGLFWError(int errCode, char const* msg)
{
    std::cerr << "ERROR " << errCode << ": " << msg << std::endl;
}

void app::runFragmentDemo(std::string const& demoName)
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


    program prog;
    CHECK(prog.create(), prog.lastError(), return;);

    // Shaders
    shader vertexShader, fragShader;
    CHECK(vertexShader.compile(readAllText("shaders/fragment_demo.vert"), GL_VERTEX_SHADER), vertexShader.lastError(), return;);
    CHECK(fragShader.compile(readAllText("shaders/" + demoName + ".frag"), GL_FRAGMENT_SHADER, IncludeCommonCode::Yes), fragShader.lastError(), return;);

    prog.attach(vertexShader);
    prog.attach(fragShader);
    CHECK(prog.link(), prog.lastError(), return;);


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


app& App()
{
    return app::inst();
}
