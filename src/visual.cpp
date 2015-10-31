#include "visual.h"
#include <array>
#include <ctime>
#include <iostream>
#include <list>
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <AntTweakBar.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "camera/camera.h"
#include "data/cube.h"
#include "shaders/program.h"
#include "shaders/shader.h"
#include "textures/texture.h"
#include "util/checked_call.h"


static size_t sWinWidth = 1280;
static size_t sWinHeight = 800;
static float sScreenRatio = static_cast<float>(sWinWidth) / static_cast<float>(sWinHeight);
static bool sMouseVisible{ false };
glm::vec3 const sCubePos(0.0f, 0.0f, 0.0f);
glm::vec3 sRotAngles;

static camera sCamera(sWinWidth, sWinHeight);
static std::vector<bool> sKeys(GLFW_KEY_LAST, false);
static float sStep = 1.0f;

static TwBar* sATB{ nullptr };

static void glfwErrorReporting(int errCode, char const* msg);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);
static void mouseCallback(GLFWwindow* window, double x, double y);
static void scrollCallback(GLFWwindow* window, double xDiff, double yDiff);
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers);
static void charCallback(GLFWwindow* window, unsigned int symb);
static void windowSizeCallback(GLFWwindow* window, int sizeX, int sizeY);
static void moveCamera(float dt);


int runVisual()
{
    sCamera.fov(90);
    CHECK(GL_FALSE != glfwInit(), "GLFW init failed", return -1;);
        // Provide some hints for the GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    glfwSetErrorCallback(glfwErrorReporting);

        // Create a window
    GLFWwindow* window = glfwCreateWindow(sWinWidth, sWinHeight, "Renderize", nullptr, nullptr);
    CHECK(window, "Error creating window", { glfwTerminate(); return -1; });
    glfwMakeContextCurrent(window);


    if (TwInit(TW_OPENGL_CORE, NULL))
    {
        TwWindowSize(sWinWidth, sWinHeight);
        sATB = TwNewBar("Tweak");
        TwAddVarRW(sATB, "step", TW_TYPE_FLOAT, &sStep, " label='Step' step=0.1 ");
    }
    else
        std::cerr << TwGetLastError() << std::endl;


        // Initialize Glew
    glewExperimental = GL_TRUE;
    GLenum glewCode = glewInit();
    CHECK(GLEW_OK == glewCode, "Failed to initialize GLEW: ",
        std::cerr << glewGetErrorString(glewCode) << std::endl; return -1;);

    glViewport(0, 0, sWinWidth, sWinHeight);
    glEnable(GL_DEPTH_TEST);

    // Initialize some GLFW callbacks
    glfwSetInputMode(window, GLFW_CURSOR, sMouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);


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
    glBufferData(GL_ARRAY_BUFFER, sizeof(sVertices), sVertices.data(), GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    }


    program prog;
    CHECK(prog.create(), prog.lastError(), return -1;);

        // Shaders
    shader vertexShader, fragShader;
    CHECK(vertexShader.compile(readAllText("shaders/cube.vert"), GL_VERTEX_SHADER), vertexShader.lastError(), return -1;);
    CHECK(fragShader.compile(readAllText("shaders/form.frag"), GL_FRAGMENT_SHADER), fragShader.lastError(), return -1;);

    prog.attach(vertexShader);
    prog.attach(fragShader);
    CHECK(prog.link(), prog.lastError(), return -1;);

    texture tex;
    CHECK(tex.load("../tex/crate.png", true), "Error loading texture", );


    std::mt19937 randGen(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    std::uniform_real_distribution<float>  uniDist;
    std::exponential_distribution<float>  expDist;

    float lastTime = static_cast<float>(glfwGetTime());
    float dt{ 0.0f };
    float const dT{ 0.0125f };
        // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float const curTime = static_cast<float>(glfwGetTime());
        dt = curTime - lastTime;
        if (dt < dT)
            continue;
        lastTime = curTime;

        //std::array<GLfloat, sNumVertices> verticesCopy = sVertices;
        //for (size_t i = 0; i < sNumVertices; i += 8)
        //{
        //    verticesCopy[i] = std::sin(curTime) * sVertices[i];
        //}
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCopy), verticesCopy.data(), GL_STATIC_DRAW);
        moveCamera(dt);

        glm::mat4 view;
        view = sCamera.view();
        glm::mat4 projection;
        projection = sCamera.projection();
        glm::mat4 model;
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::translate(model, sCubePos);
        model = glm::rotate(model, glm::radians(sRotAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(sRotAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(sRotAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));

        prog.use();
        prog["model"] = model;
        prog["view"] = view;
        prog["projection"] = projection;
        prog["iResolution"] = glm::vec2(sWinWidth, sWinHeight);
        prog["iGlobalTime"] = curTime;

        // Rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        //TwDraw();

        glfwSwapBuffers(window);
    }

    TwDeleteBar(sATB);
    sATB = nullptr;

    glfwTerminate();
    return 0;
}

void glfwErrorReporting(int errCode, char const* msg)
{
    std::cerr << "ERROR " << errCode << ": " << msg << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
    if (TwEventKeyGLFW(key, action))
        return;
    if (action == GLFW_PRESS)
        sKeys[key] = true;
    if (action == GLFW_RELEASE)
        sKeys[key] = false;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    else if ((key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) && action == GLFW_PRESS)
    {
        sMouseVisible = !sMouseVisible;
        glfwSetInputMode(window, GLFW_CURSOR, sMouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}

void mouseCallback(GLFWwindow* window, double x, double y)
{
    float const xFloat = static_cast<GLfloat>(x);
    float const yFloat = static_cast<GLfloat>(y);
    static GLfloat lastX = xFloat, lastY = yFloat;

    if (sMouseVisible)
    {
        lastX = xFloat;
        lastY = yFloat;
        TwEventMousePosGLFW(static_cast<int>(x), static_cast<int>(y));
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

void scrollCallback(GLFWwindow* window, double xDiff, double yDiff)
{
    if (TwEventMouseWheelGLFW(static_cast<int>(yDiff)))
        return;
    sCamera.fov(sCamera.fov() - static_cast<float>(yDiff));
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers)
{
    TwEventMouseButtonGLFW(button, action);
}

void charCallback(GLFWwindow* window, unsigned int symb)
{
    TwEventCharGLFW(symb, GLFW_PRESS);
}

void windowSizeCallback(GLFWwindow* window, int sizeX, int sizeY)
{
    sWinWidth = sizeX;
    sWinHeight = sizeY;
    glViewport(0, 0, sWinWidth, sWinHeight);
    TwWindowSize(sWinWidth, sWinHeight);
}

void moveCamera(float dt)
{
    if (sMouseVisible)
        return;
    float const rotAngle = 90.0f * dt;
    if (sKeys[GLFW_KEY_W])
        sRotAngles.x += rotAngle;
    if (sKeys[GLFW_KEY_S])
        sRotAngles.x -= rotAngle;
    if (sKeys[GLFW_KEY_A])
        sRotAngles.y -= rotAngle;
    if (sKeys[GLFW_KEY_D])
        sRotAngles.y += rotAngle;
    if (sKeys[GLFW_KEY_SPACE])
        sRotAngles.z += rotAngle;
    if (sKeys[GLFW_KEY_LEFT_SHIFT] || sKeys[GLFW_KEY_RIGHT_SHIFT])
        sRotAngles.z -= rotAngle;
}
