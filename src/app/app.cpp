#include <iostream>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "app.h"
#include "data/light.h"
#include "data/model.h"
#include "shaders/program.h"
#include "util/checked_call.h"
#include "util/date.h"


namespace
{
    App*  sAppInstance{ nullptr };

    void glfwErrorReporting(int errCode, char const* msg)
    {
        APP().onGLFWError(errCode, msg);
    }

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
    {
        if (action == GLFW_PRESS)
            APP().keyDown(key);
        else if (action == GLFW_RELEASE)
            APP().keyUp(key);
    }

    void mouseCallback(GLFWwindow* window, double x, double y)
    {
        APP().mouse(glm::vec2(static_cast<float>(x), static_cast<float>(y)));
    }

    void scrollCallback(GLFWwindow* window, double xDiff, double yDiff)
    {
        APP().scroll(static_cast<float>(xDiff), static_cast<float>(yDiff));
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers)
    {
        APP().touchDown(button);
    }

    void charCallback(GLFWwindow* window, unsigned int symb)
    {
    }

    void windowSizeCallback(GLFWwindow* window, int sizeX, int sizeY)
    {
        APP().resize(sizeX, sizeY);
    }
} // anonymous namespace



void App::create()
{
    if (!sAppInstance)
        sAppInstance = new App();
}

void App::destroy()
{
    if (sAppInstance)
    {
        delete sAppInstance;
        sAppInstance = nullptr;
    }
}

App& App::inst()
{
    return *sAppInstance;
}

App::App()
    : mWinSize(800, 800)
    , mWindow(nullptr)
    , mKeys(GLFW_KEY_LAST, false)
    , mCamera(mWinSize.x, mWinSize.y)
{
    mCamera.far(120.0f);
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
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);
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

bool App::shouldClose() const
{
    return glfwWindowShouldClose(mWindow) != 0;
}

void App::resize(int width, int height)
{
    mWinSize = glm::ivec2(width, height);
    glViewport(0, 0, mWinSize.x, mWinSize.y);
}

void App::mouse(glm::vec2 const& pos)
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

    mYaw += xDiff;
    mPitch = glm::clamp(mPitch - yDiff, -89.0f, 89.0f);
}

void App::scroll(float xDiff, float yDiff)
{
    mCamera.fov(mCamera.fov() - static_cast<float>(yDiff));
}

void App::touchDown(int button)
{
}

void App::touchUp(int button)
{
}

void App::keyDown(int key)
{
    mKeys[key] = true;
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(mWindow, GL_TRUE);
    else if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT)
    {
        mMouseVisible = !mMouseVisible;
        glfwSetInputMode(mWindow, GLFW_CURSOR, mMouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}

void App::keyUp(int key)
{
    mKeys[key] = false;
}

bool App::isPressed(int key)
{
    return mKeys[key];
}

void App::onGLFWError(int errCode, char const* msg)
{
    std::cerr << "ERROR " << errCode << ": " << msg << std::endl;
}

void App::run()
{
    DirLight dirLight{
        { 1.0f, 1.0f, -0.3f },
        { 0.1f, 0.1f, 0.1f },
        { 0.8f, 0.8f, 0.8f },
        { 0.4f, 0.4f, 0.4f },
    };
    PointLight pLight
    {
        { -1.0f, -1.0f, 0.0f },
        { 0.1f, 0.1f, 0.1f },
        { 0.3f, 0.02f, 0.02f },
        { 0.5f, 0.1f, 0.1f },
        1.0f, 0.09f, 0.05f
    };
    SpotLight sLight;
    sLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    sLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    sLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    sLight.constCoeff = pLight.constCoeff;
    sLight.linCoeff = pLight.linCoeff;
    sLight.quadCoeff = pLight.quadCoeff;
    sLight.cutOff = 0.05f;
    sLight.outerCutOff = 0.2f;


    Program prog, scProg;
    CHECK(prog.create(), prog.lastError(), return;);
    CHECK(scProg.create(), prog.lastError(), return;);

    // Shaders
    Shader vertexShader, fragShader, solColShader;
    CHECK(vertexShader.compile(readAllText("shaders/simple.vert"), GL_VERTEX_SHADER), vertexShader.lastError(), return;);
    CHECK(fragShader.compile(readAllText("shaders/simple.frag"), GL_FRAGMENT_SHADER,
        IncludeCommonCode::No), fragShader.lastError(), return;);
    CHECK(solColShader.compile(readAllText("shaders/solid_color.frag"), GL_FRAGMENT_SHADER,
        IncludeCommonCode::No), solColShader.lastError(), return;);

    prog.attach(vertexShader);
    prog.attach(fragShader);
    CHECK(prog.link(), prog.lastError(), return;);

    scProg.attach(vertexShader);
    scProg.attach(solColShader);
    CHECK(scProg.link(), scProg.lastError(), return;);

    Model model("nanosuit/nanosuit.obj");

    glEnable(GL_STENCIL_TEST);

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

        moveCamera(dt);

        sLight.position = mCamera.pos();
        sLight.direction = mCamera.front();

        // Rendering
        glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        prog.use();
        auto scaleMat = glm::scale(glm::mat4(), glm::vec3(0.2f));
        auto transVec = glm::vec3(0.0f, -8.0f, 0.0f);
        prog["model"] = glm::translate(scaleMat, transVec);
        prog["view"] = mCamera.view();
        prog["projection"] = mCamera.projection();
        prog["viewerPos"] = mCamera.pos();

        prog["dirLight.direction"] = dirLight.direction;
        prog["dirLight.ambient"] = dirLight.ambient;
        prog["dirLight.diffuse"] = dirLight.diffuse;
        prog["dirLight.specular"] = dirLight.specular;
        prog["pLight.position"] = pLight.position;
        prog["pLight.ambient"] = pLight.ambient;
        prog["pLight.diffuse"] = pLight.diffuse;
        prog["pLight.specular"] = pLight.specular;
        prog["pLight.constCoeff"] = pLight.constCoeff;
        prog["pLight.linCoeff"] = pLight.linCoeff;
        prog["pLight.quadCoeff"] = pLight.quadCoeff;
        prog["spLight.position"] = sLight.position;
        prog["spLight.direction"] = sLight.direction;
        prog["spLight.ambient"] = sLight.ambient;
        prog["spLight.diffuse"] = sLight.diffuse;
        prog["spLight.specular"] = sLight.specular;
        prog["spLight.constCoeff"] = sLight.constCoeff;
        prog["spLight.linCoeff"] = sLight.linCoeff;
        prog["spLight.quadCoeff"] = sLight.quadCoeff;
        prog["spLight.cutOff"] = glm::cos(sLight.cutOff);
        prog["spLight.outerCutOff"] = glm::cos(sLight.outerCutOff);

        //glm::vec4 iDate(0.0f, 0.0f, 0.0f, secFrom00());
        //prog["iResolution"] = glm::vec3(mWinSize, 0.0);
        //prog["iGlobalTime"] = curTime;
        //prog["iDate"] = iDate;

        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        model.draw(prog);


        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        scProg.use();
        scaleMat = glm::scale(glm::mat4(), glm::vec3(0.205f));
        scProg["model"] = glm::translate(scaleMat, transVec);
        scProg["view"] = mCamera.view();
        scProg["projection"] = mCamera.projection();
        scProg["uColor"] = glm::vec4(0.1f, 0.7f, 0.11f, 1.0f);
        model.draw(scProg);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

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


    Program prog;
    CHECK(prog.create(), prog.lastError(), return;);

    // Shaders
    Shader vertexShader, fragShader;
    CHECK(vertexShader.compile(readAllText("shaders/fragment_demo.vert"), GL_VERTEX_SHADER), vertexShader.lastError(), return;);
    CHECK(fragShader.compile(readAllText("shaders/" + demoName + ".frag"), GL_FRAGMENT_SHADER,
        IncludeCommonCode::Yes), fragShader.lastError(), return;);

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

void App::moveCamera(float dt)
{
    GLfloat cameraSpeed = 5.0f * dt;
    auto camPos = mCamera.pos();
    if (isPressed(GLFW_KEY_W))
        camPos += cameraSpeed * mCamera.front();
    if (isPressed(GLFW_KEY_S))
        camPos -= cameraSpeed * mCamera.front();
    if (isPressed(GLFW_KEY_A))
        camPos -= glm::normalize(glm::cross(mCamera.front(), mCamera.up())) * cameraSpeed;
    if (isPressed(GLFW_KEY_D))
        camPos += glm::normalize(glm::cross(mCamera.front(), mCamera.up())) * cameraSpeed;
    if (isPressed(GLFW_KEY_SPACE))
        camPos += glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * cameraSpeed;
    if (isPressed(GLFW_KEY_LEFT_SHIFT) || isPressed(GLFW_KEY_RIGHT_SHIFT))
        camPos -= glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * cameraSpeed;
    mCamera.pos(camPos);
    mCamera.front(mPitch, mYaw);
}


App& APP()
{
    return App::inst();
}
