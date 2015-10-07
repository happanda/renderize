#include <iostream>
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
#include "SOIL.h"


size_t const sWinWidth = 600;
size_t const sWinHeight = 600;
camera sCamera(sWinWidth, sWinHeight);
float sYaw = 0.0f;
float sPitch = 0.0f;
std::vector<bool> sKeys(GLFW_KEY_LAST, false);
glm::vec3 sLightPos(1.2f, 1.0f, 2.0f);
glm::vec3 sLightColor(1.0f, 1.0f, 1.0f);

void glfwErrorReporting(int errCode, char const* msg);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);
void mouseCallback(GLFWwindow* window, double x, double y);
void scrollCallback(GLFWwindow* window, double xDiff, double yDiff);
void moveCamera(float dt);

int main(int argc, char* argv[])
{
    if (GL_FALSE == glfwInit())
        return -1;
    // Provide some hints for the GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);


    program shaderCube;
    program shaderLamp;
    if (!shaderCube.create())
    {
        std::cerr << shaderCube.lastError() << std::endl;
        return -1;
    }
    if (!shaderLamp.create())
    {
        std::cerr << shaderLamp.lastError() << std::endl;
        return -1;
    }

    {
        // Shaders
        shader vertexShader;
        if (!vertexShader.compile(readAllText("../../src/shaders/simple.vert"), GL_VERTEX_SHADER))
        {
            std::cerr << vertexShader.lastError() << std::endl;
            return -1;
        }
        shader fragCube;
        if (!fragCube.compile(readAllText("../../src/shaders/simple.frag"), GL_FRAGMENT_SHADER))
        {
            std::cerr << fragCube.lastError() << std::endl;
            return -1;
        }
        shader fragLamp;
        if (!fragLamp.compile(readAllText("../../src/shaders/lamp.frag"), GL_FRAGMENT_SHADER))
        {
            std::cerr << fragLamp.lastError() << std::endl;
            return -1;
        }

        // Shader program
        shaderCube.attach(vertexShader);
        shaderCube.attach(fragCube);
        if (!shaderCube.link())
        {
            std::cerr << shaderCube.lastError() << std::endl;
            return -1;
        }
        shaderLamp.attach(vertexShader);
        shaderLamp.attach(fragLamp);
        if (!shaderLamp.link())
        {
            std::cerr << shaderLamp.lastError() << std::endl;
            return -1;
        }
    }

    // Texture loading
    /*GLuint textures[2];
    glGenTextures(2, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int texWidth{ 0 }, texHeight{ 0 };
    auto image = SOIL_load_image("../../tex/crate.jpg", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image = SOIL_load_image("../../tex/awesomeface.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);*/


    // Geometry to draw
    // Cubes positions
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };
    // Cube data
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };


    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);
    }
    glBindVertexArray(0);


    float lastTime = static_cast<float>(glfwGetTime());
    float dt = 0.0f;
    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float const curTime = static_cast<float>(glfwGetTime());
        dt = curTime - lastTime;
        lastTime = curTime;

        moveCamera(dt);

        // Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 view;
        view = sCamera.view();
        glm::mat4 projection;
        projection = sCamera.projection();

        {
            shaderLamp.use();
            GLint lightColorLocLamp = glGetUniformLocation(shaderLamp, "lightColor");
            glUniform3f(lightColorLocLamp, sLightColor.x, sLightColor.y, sLightColor.z);

            glm::mat4 model;
            model = glm::translate(model, sLightPos);
            model = glm::scale(model, glm::vec3(0.2f));
            glBindVertexArray(lightVAO);
            GLint modelLocation = glGetUniformLocation(shaderLamp, "model");
            GLint viewLocation = glGetUniformLocation(shaderLamp, "view");
            GLint projectionLocation = glGetUniformLocation(shaderLamp, "projection");
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        {
            shaderCube.use();
            GLint modelLocation = glGetUniformLocation(shaderCube, "model");
            GLint viewLocation = glGetUniformLocation(shaderCube, "view");
            GLint projectionLocation = glGetUniformLocation(shaderCube, "projection");
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
            GLint objColorLoc = glGetUniformLocation(shaderCube, "objectColor");
            GLint lightColorLoc = glGetUniformLocation(shaderCube, "lightColor");
            glUniform3f(objColorLoc, 0.5f, 0.5f, 0.11f);
            glUniform3f(lightColorLoc, sLightColor.x, sLightColor.y, sLightColor.z);


            glBindVertexArray(VAO);
            for (int i = 0; i < 10; ++i)
            {
                glm::mat4 model;
                model = glm::translate(model, cubePositions[i]);
                GLfloat angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle) + curTime, glm::vec3(1.0f, 0.1f * i, 0.5f));
                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        sCamera.fov(sCamera.fov() + 5);
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        sCamera.fov(sCamera.fov() - 5);
    }
}

void mouseCallback(GLFWwindow* window, double x, double y)
{
    float const xFloat = static_cast<GLfloat>(x);
    float const yFloat = static_cast<GLfloat>(y);

    static GLfloat lastX = xFloat, lastY = yFloat;
    GLfloat xDiff = xFloat - lastX;
    GLfloat yDiff = yFloat - lastY;

    lastX = xFloat;
    lastY = yFloat;

    GLfloat sensitivity = 0.08f;
    xDiff *= sensitivity;
    yDiff *= sensitivity;

    sYaw += xDiff;
    sPitch = glm::clamp(sPitch - yDiff, -89.0f, 89.0f);
}

void scrollCallback(GLFWwindow* window, double xDiff, double yDiff)
{
    sCamera.fov(sCamera.fov() - static_cast<float>(yDiff));
}

void moveCamera(float dt)
{
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
    sCamera.pos(camPos);
    sCamera.front(sPitch, sYaw);
}