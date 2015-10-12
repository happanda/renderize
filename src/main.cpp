#include <iostream>
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

#include "camera/camera.h"
#include "shaders/program.h"
#include "SOIL.h"


size_t sWinWidth = 500;
size_t sWinHeight = 500;
TwBar* sATB{ nullptr };
bool sMouseVisible{ false };

camera sCamera(sWinWidth, sWinHeight);
float sYaw = 0.0f;
float sPitch = 0.0f;
std::vector<bool> sKeys(GLFW_KEY_LAST, false);
struct material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};
struct light
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

material sCube{
    { 0.1f, 0.1f, 0.1f },
    { 0.3f, 0.4f, 0.6f },
    { 0.4f, 0.1f, 0.6f },
    32.0f };
light sLight{
    { 8.0f, 1.0f, 2.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
};


void glfwErrorReporting(int errCode, char const* msg);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);
void mouseCallback(GLFWwindow* window, double x, double y);
void scrollCallback(GLFWwindow* window, double xDiff, double yDiff);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers);
void charCallback(GLFWwindow* window, unsigned int symb);
void windowSizeCallback(GLFWwindow* window, int sizeX, int sizeY);
void moveCamera(float dt);


int main(int argc, char* argv[])
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


        // AntTweakBar
    if (TwInit(TW_OPENGL_CORE, NULL))
    {
        TwWindowSize(sWinWidth, sWinHeight);
        sATB = TwNewBar("Tweak");

        TwAddVarRW(sATB, "mat.amb", TW_TYPE_COLOR3F, glm::value_ptr(sCube.ambient), " label='Ambient color' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "mat.dif", TW_TYPE_COLOR3F, glm::value_ptr(sCube.diffuse), " label='Diffuse color' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "mat.spe", TW_TYPE_COLOR3F, glm::value_ptr(sCube.specular), " label='Specular color' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "mat.shi", TW_TYPE_FLOAT, &sCube.shininess, " label='Shininess' min=-32 max=512 step=1 ");

        TwAddVarRW(sATB, "light.pos.x", TW_TYPE_FLOAT, &sLight.position.x, " label='Light pos x' step=0.1 ");
        TwAddVarRW(sATB, "light.pos.y", TW_TYPE_FLOAT, &sLight.position.y, " label='Light pos y' step=0.1 ");
        TwAddVarRW(sATB, "light.pos.z", TW_TYPE_FLOAT, &sLight.position.z, " label='Light pos z' step=0.1 ");
        TwAddVarRW(sATB, "light.amb", TW_TYPE_COLOR3F, glm::value_ptr(sLight.ambient), " label='Ambient color' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "light.dif", TW_TYPE_COLOR3F, glm::value_ptr(sLight.diffuse), " label='Diffuse color' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "light.spe", TW_TYPE_COLOR3F, glm::value_ptr(sLight.specular), " label='Specular color' min=-3.0 max=3.0 step=0.05 ");
    }
    else
        std::cerr << TwGetLastError() << std::endl;


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
        if (!vertexShader.compile(readAllText("shaders/cube.vert"), GL_VERTEX_SHADER))
        {
            std::cerr << vertexShader.lastError() << std::endl;
            return -1;
        }
        shader fragCube;
        if (!fragCube.compile(readAllText("shaders/cube.frag"), GL_FRAGMENT_SHADER))
        {
            std::cerr << fragCube.lastError() << std::endl;
            return -1;
        }
        shader fragLamp;
        if (!fragLamp.compile(readAllText("shaders/lamp.frag"), GL_FRAGMENT_SHADER))
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
    GLuint textures[2];
    glGenTextures(2, textures);

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int texWidth{ 0 }, texHeight{ 0 };
    auto image = SOIL_load_image("../tex/crate.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    image = SOIL_load_image("../tex/awesomeface.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);


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
        // Positions           // Normals           // Texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    }

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid*>(0));
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

        glm::vec4 actualLightPos;
        {
            shaderLamp.use();
            GLint lightPosLoc = glGetUniformLocation(shaderCube, "light.position");
            glUniform3f(lightPosLoc, sLight.position.x, sLight.position.y, sLight.position.z);
            GLint lightAmbLoc = glGetUniformLocation(shaderCube, "light.ambient");
            glUniform3f(lightAmbLoc, sLight.ambient.x, sLight.ambient.y, sLight.ambient.z);
            GLint lightDifLoc = glGetUniformLocation(shaderCube, "light.diffuse");
            glUniform3f(lightDifLoc, sLight.diffuse.x, sLight.diffuse.y, sLight.diffuse.z);
            GLint lightSpeLoc = glGetUniformLocation(shaderCube, "light.specular");
            glUniform3f(lightSpeLoc, sLight.specular.x, sLight.specular.y, sLight.specular.z);

            glm::mat4 model;
            //model = glm::rotate(model, curTime, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, sLight.position);
            model = glm::scale(model, glm::vec3(0.2f));
            actualLightPos = model * glm::vec4(sLight.position, 1.0f);
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
            GLint viewerPosLoc = glGetUniformLocation(shaderCube, "viewerPos");
            glUniform3f(viewerPosLoc, sCamera.pos().x, sCamera.pos().y, sCamera.pos().z);

            GLint matDifLoc = glGetUniformLocation(shaderCube, "material.diffuse");
            glUniform1i(matDifLoc, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0]);

            GLint matSpeLoc = glGetUniformLocation(shaderCube, "material.specular");
            glUniform3f(matSpeLoc, sCube.specular.x, sCube.specular.y, sCube.specular.z);
            GLint matShiLoc = glGetUniformLocation(shaderCube, "material.shininess");
            glUniform1f(matShiLoc, sCube.shininess);

            GLint lightPosLoc = glGetUniformLocation(shaderCube, "light.position");
            glUniform3f(lightPosLoc, sLight.position.x, sLight.position.y, sLight.position.z);
            GLint lightAmbLoc = glGetUniformLocation(shaderCube, "light.ambient");
            glUniform3f(lightAmbLoc, sLight.ambient.x, sLight.ambient.y, sLight.ambient.z);
            GLint lightDifLoc = glGetUniformLocation(shaderCube, "light.diffuse");
            glUniform3f(lightDifLoc, sLight.diffuse.x, sLight.diffuse.y, sLight.diffuse.z);
            GLint lightSpeLoc = glGetUniformLocation(shaderCube, "light.specular");
            glUniform3f(lightSpeLoc, sLight.specular.x, sLight.specular.y, sLight.specular.z);

            glBindVertexArray(VAO);
            for (int i = 0; i < 10; ++i)
            {
                glm::mat4 model;
                model = glm::translate(model, cubePositions[i]);
                GLfloat angle = 20.0f * i;
                //model = glm::rotate(model, glm::radians(angle) + curTime, glm::vec3(1.0f, 0.1f * i, 0.5f));
                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        TwDraw();

        glfwSwapBuffers(window);
    }

    TwDeleteBar(sATB);
    sATB = nullptr;

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

    sYaw += xDiff;
    sPitch = glm::clamp(sPitch - yDiff, -89.0f, 89.0f);
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
    sCamera.front(sPitch, sYaw);
}