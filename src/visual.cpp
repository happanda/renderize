#include "visual.h"
#include <ctime>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera/camera.h"
#include "shaders/program.h"
#include "shaders/shader.h"


static size_t sWinWidth = 800;
static size_t sWinHeight = 800;
static bool sMouseVisible{ false };

static camera sCamera(sWinWidth, sWinHeight);
static std::vector<bool> sKeys(GLFW_KEY_LAST, false);


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
    if (GL_FALSE == glfwInit())
        return -1;
        // Provide some hints for the GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    glfwSetErrorCallback(glfwErrorReporting);

        // Create a window
    GLFWwindow* window = glfwCreateWindow(sWinWidth, sWinHeight, "Renderize", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

        // Initialize Glew
    glewExperimental = GL_TRUE;
    GLenum glewCode = glewInit();
    if (GLEW_OK != glewCode)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewCode) << std::endl;
        return -1;
    }

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);
    }


    program prog;
    if (!prog.create())
    {
        std::cerr << prog.lastError() << std::endl;
        return -1;
    }

        // Shaders
    shader vertexShader;
    if (!vertexShader.compile(readAllText("shaders/simple.vert"), GL_VERTEX_SHADER))
    {
        std::cerr << vertexShader.lastError() << std::endl;
        return -1;
    }
    shader fragShader;
    if (!fragShader.compile(readAllText("shaders/visual.frag"), GL_FRAGMENT_SHADER))
    {
        std::cerr << fragShader.lastError() << std::endl;
        return -1;
    }

    prog.attach(vertexShader);
    prog.attach(fragShader);
    if (!prog.link())
    {
        std::cerr << prog.lastError() << std::endl;
        return -1;
    }


    std::mt19937 randGen(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    std::uniform_real_distribution<float>  uniDist;


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
        
        //moveCamera(dt);

        prog.use();
        prog["sourcePos"] = glm::vec2(sWinWidth / 2.0f, sWinHeight / 2.0f);
        prog["dt"] = dt;
        prog["curTime"] = curTime;
        prog["randColor"] = glm::vec3(uniDist(randGen), uniDist(randGen), uniDist(randGen));

        // Rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void glfwErrorReporting(int errCode, char const* msg)
{
    std::cerr << "ERROR " << errCode << ": " << msg << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
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
    sCamera.fov(sCamera.fov() - static_cast<float>(yDiff));
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers)
{
}

void charCallback(GLFWwindow* window, unsigned int symb)
{
}

void windowSizeCallback(GLFWwindow* window, int sizeX, int sizeY)
{
    sWinWidth = sizeX;
    sWinHeight = sizeY;
    glViewport(0, 0, sWinWidth, sWinHeight);
}

void moveCamera(float dt)
{
    if (sMouseVisible)
        return;
    GLfloat cameraSpeed = 5.0f * dt;
    auto camPos = sCamera.pos();
    if (sKeys[GLFW_KEY_W])
        camPos += cameraSpeed * sCamera.front();
    if (sKeys[GLFW_KEY_S])
        camPos -= cameraSpeed * sCamera.front();
    if (sKeys[GLFW_KEY_A])
        camPos -= glm::normalize(glm::cross(sCamera.front(), sCamera.up())) * cameraSpeed;
    if (sKeys[GLFW_KEY_D])
        camPos += glm::normalize(glm::cross(sCamera.front(), sCamera.up())) * cameraSpeed;
    if (sKeys[GLFW_KEY_SPACE])
        camPos += glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * cameraSpeed;
    if (sKeys[GLFW_KEY_LEFT_SHIFT] || sKeys[GLFW_KEY_RIGHT_SHIFT])
        camPos -= glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)) * cameraSpeed;
    sCamera.pos(camPos);
    //sCamera.front(sPitch, sYaw);
}
