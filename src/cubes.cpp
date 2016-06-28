#include "cubes.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <AntTweakBar.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "camera/camera.h"
#include "data/cube.h"
#include "shaders/program.h"
#include "buffers/frame_buffer.h"
#include "buffers/texture.h"
#include "util/checked_call.h"


size_t sWinWidth = 1280;
size_t sWinHeight = 800;
static TwBar* sATB{ nullptr };
bool sMouseVisible{ false };

bool sUseCam1{ true };
Camera sCamera(sWinWidth, sWinHeight);
Camera sCamera2(sWinWidth, sWinHeight);
float sCamDist{ 5.0f };
float sCamAngle{ 0.01f };

float sYaw = 0.0f;
float sPitch = 0.0f;
std::vector<bool> sKeys(GLFW_KEY_LAST, false);
struct material
{
    float shininess;
};
struct dir_light
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
struct point_light
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constCoeff;
    float linCoeff;
    float quadCoeff;
};
struct spot_light : public point_light
{
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
};

material sCube{ 128.0f };
dir_light sDirLight{
    { 1.0f, 1.0f, -0.3f },
    { 0.1f, 0.1f, 0.1f },
    { 0.8f, 0.8f, 0.8f },
    { 0.4f, 0.4f, 0.4f },
};
point_light sPLight
{
    { -1.0f, -1.0f, 0.0f },
    { 0.1f, 0.1f, 0.1f },
    { 0.3f, 0.02f, 0.02f },
    { 0.5f, 0.1f, 0.1f },
    1.0f, 0.09f, 0.05f
};
spot_light sSPLight;


void glfwErrorReporting(int errCode, char const* msg);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers);
void mouseCallback(GLFWwindow* window, double x, double y);
void scrollCallback(GLFWwindow* window, double xDiff, double yDiff);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int modifiers);
void charCallback(GLFWwindow* window, unsigned int symb);
void windowSizeCallback(GLFWwindow* window, int sizeX, int sizeY);
void moveCamera(float dt);

int runCubes()
{
    sSPLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    sSPLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    sSPLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    sSPLight.constCoeff = sPLight.constCoeff;
    sSPLight.linCoeff = sPLight.linCoeff;
    sSPLight.quadCoeff = sPLight.quadCoeff;
    sSPLight.cutOff = 0.05f;
    sSPLight.outerCutOff = 0.2f;

    CHECK(GL_TRUE == glfwInit(), "GLFW init failed", return -1;);
        // Provide some hints for the GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Initialize some GLFW callbacks
    glfwSetErrorCallback(glfwErrorReporting);

        // Create a window
    GLFWwindow* window = glfwCreateWindow(sWinWidth, sWinHeight, "Renderize", nullptr, nullptr);
    CHECK(window, "Error creating window", { glfwTerminate(); return -1; });
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, sMouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCharCallback(window, charCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);


        // AntTweakBar
    if (TwInit(TW_OPENGL_CORE, NULL))
    {
        TwWindowSize(sWinWidth, sWinHeight);
        sATB = TwNewBar("Tweak");

        TwAddVarRW(sATB, "dist", TW_TYPE_FLOAT, &sCamDist, " label='Dist' step=0.1 precision=2");
        TwAddVarRW(sATB, "angle", TW_TYPE_FLOAT, &sCamAngle, " label='Angle' step=0.005 precision=3");

        /*
        TwAddVarRW(sATB, "mat.shi", TW_TYPE_FLOAT, &sCube.shininess, " label='Shininess' min=-32 max=512 step=1 ");
        
        TwAddVarRW(sATB, "dlight.dir", TW_TYPE_DIR3F, glm::value_ptr(sDirLight.direction), " label='DLight dir' ");
        TwAddVarRW(sATB, "dlight.amb", TW_TYPE_COLOR3F, glm::value_ptr(sDirLight.ambient), " label='DLight amb' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "dlight.dif", TW_TYPE_COLOR3F, glm::value_ptr(sDirLight.diffuse), " label='DLight diff' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "dlight.spe", TW_TYPE_COLOR3F, glm::value_ptr(sDirLight.specular), " label='DLight spec' min=-3.0 max=3.0 step=0.05 ");

        TwAddVarRW(sATB, "plight.pos.x", TW_TYPE_FLOAT, &sPLight.position.x, " label='PLight pos.x' ");
        TwAddVarRW(sATB, "plight.pos.y", TW_TYPE_FLOAT, &sPLight.position.y, " label='PLight pos.y' ");
        TwAddVarRW(sATB, "plight.pos.z", TW_TYPE_FLOAT, &sPLight.position.z, " label='PLight pos.z' ");
        TwAddVarRW(sATB, "plight.amb", TW_TYPE_COLOR3F, glm::value_ptr(sPLight.ambient), " label='PLight amb' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "plight.dif", TW_TYPE_COLOR3F, glm::value_ptr(sPLight.diffuse), " label='PLight diff' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "plight.spe", TW_TYPE_COLOR3F, glm::value_ptr(sPLight.specular), " label='PLight spec' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "plight.const", TW_TYPE_FLOAT, &sPLight.constCoeff, " label='PLight const' step=0.05 ");
        TwAddVarRW(sATB, "plight.lin", TW_TYPE_FLOAT, &sPLight.linCoeff, " label='PLight lin' step=0.05 ");
        TwAddVarRW(sATB, "plight.quad", TW_TYPE_FLOAT, &sPLight.quadCoeff, " label='PLight quad' step=0.05 ");

        TwAddVarRW(sATB, "splight.pos.x", TW_TYPE_FLOAT, &sSPLight.position.x, " label='SPLight pos.x' ");
        TwAddVarRW(sATB, "splight.pos.y", TW_TYPE_FLOAT, &sSPLight.position.y, " label='SPLight pos.y' ");
        TwAddVarRW(sATB, "splight.pos.z", TW_TYPE_FLOAT, &sSPLight.position.z, " label='SPLight pos.z' ");
        TwAddVarRW(sATB, "splight.dir", TW_TYPE_DIR3F, glm::value_ptr(sSPLight.direction), " label='SPLight dir' ");
        TwAddVarRW(sATB, "splight.amb", TW_TYPE_COLOR3F, glm::value_ptr(sSPLight.ambient), " label='SPLight amb' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "splight.dif", TW_TYPE_COLOR3F, glm::value_ptr(sSPLight.diffuse), " label='SPLight diff' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "splight.spe", TW_TYPE_COLOR3F, glm::value_ptr(sSPLight.specular), " label='SPLight spec' min=-3.0 max=3.0 step=0.05 ");
        TwAddVarRW(sATB, "splight.const", TW_TYPE_FLOAT, &sSPLight.constCoeff, " label='SPLight const' step=0.05 ");
        TwAddVarRW(sATB, "splight.lin", TW_TYPE_FLOAT, &sSPLight.linCoeff, " label='SPLight lin' step=0.05 ");
        TwAddVarRW(sATB, "splight.quad", TW_TYPE_FLOAT, &sSPLight.quadCoeff, " label='SPLight quad' step=0.05 ");
        TwAddVarRW(sATB, "splight.cutoff", TW_TYPE_FLOAT, &sSPLight.cutOff, " label='SPLight cutoff' step=0.05 ");
        TwAddVarRW(sATB, "splight.outercutoff", TW_TYPE_FLOAT, &sSPLight.outerCutOff, " label='SPLight outerCutOff' step=0.05 ");*/
    }
    else
        std::cerr << TwGetLastError() << std::endl;


        // Initialize Glew
    glewExperimental = GL_TRUE;
    GLenum glewCode = glewInit();
    CHECK(GLEW_OK == glewCode, "Failed to initialize GLEW: ",
        std::cerr << glewGetErrorString(glewCode) << std::endl; return -1;);

    glEnable(GL_DEPTH_TEST);


    Program shaderCube;
    Program shaderLamp;
    CHECK(shaderCube.create(), shaderCube.lastError(), return -1;);
    CHECK(shaderLamp.create(), shaderLamp.lastError(), return -1;);
    {
        // Shaders
        Shader vertexShader;
        CHECK(vertexShader.compile(readAllText("shaders/cube.vert"), GL_VERTEX_SHADER), vertexShader.lastError(), return -1;);
        Shader fragCube;
        CHECK(fragCube.compile(readAllText("shaders/cube.frag"), GL_FRAGMENT_SHADER), fragCube.lastError(), return -1;);
        Shader fragLamp;
        CHECK(fragLamp.compile(readAllText("shaders/lamp.frag"), GL_FRAGMENT_SHADER), fragLamp.lastError(), return -1;);

        // Shader program
        shaderCube.attach(vertexShader);
        shaderCube.attach(fragCube);
        CHECK(shaderCube.link(), shaderCube.lastError(), return -1;);
        shaderLamp.attach(vertexShader);
        shaderLamp.attach(fragLamp);
        CHECK(shaderLamp.link(), shaderLamp.lastError(), return -1;);
    }

    Program simpleProg;
    CHECK(simpleProg.create(), simpleProg.lastError(), return -1;);
    {
        // Shaders
        Shader vertexShader;
        CHECK(vertexShader.compile(readAllText("shaders/simple.vert"), GL_VERTEX_SHADER), vertexShader.lastError(), return -1;);
        Shader fragCube;
        CHECK(fragCube.compile(readAllText("shaders/simple.frag"), GL_FRAGMENT_SHADER), fragCube.lastError(), return -1;);
        
        // Shader program
        simpleProg.attach(vertexShader);
        simpleProg.attach(fragCube);
        CHECK(simpleProg.link(), simpleProg.lastError(), return -1;);
    }

    // Texture loading
    Texture crateTex, crateSpecTex;
    CHECK(crateTex.load("../tex/crate.png", true), "Error loading crate texture", );
    CHECK(crateSpecTex.load("../tex/crate_specular.png", true), "Error loading crate specular texture", );


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
    glBindVertexArray(0);

    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);
    }
    glBindVertexArray(0);



    glm::vec3 quad[] = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f)
    };
    GLuint quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    GLuint quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glEnableVertexAttribArray(0);
    }



    FrameBuffer renderTex[2];
    CHECK(renderTex[0].create(sWinWidth, sWinHeight), "Error creating render target 0", return -1;);
    CHECK(renderTex[1].create(sWinWidth, sWinHeight), "Error creating render target 1", return -1;);

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


            // TO TEXTURE
        renderTex[0].bind();
        glViewport(0, 0, sWinWidth, sWinHeight);

        // Rendering
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view;
        view = sCamera.view();
        glm::mat4 projection;
        projection = sCamera.projection();

        shaderCube.use();
        shaderCube["view"] = view;
        shaderCube["projection"] = projection;
        shaderCube["viewerPos"] = sCamera.pos();

        shaderCube["material.diffuse"] = 0;
        crateTex.active(GL_TEXTURE0);
        shaderCube["material.specular"] = 1;
        crateSpecTex.active(GL_TEXTURE1);
        shaderCube["material.shininess"] = sCube.shininess;

        shaderCube["dirLight.direction"] = sDirLight.direction;
        shaderCube["dirLight.ambient"] = sDirLight.ambient;
        shaderCube["dirLight.diffuse"] = sDirLight.diffuse;
        shaderCube["dirLight.specular"] = sDirLight.specular;
            
        shaderCube["pLight.position"] = sPLight.position;
        shaderCube["pLight.ambient"] = sPLight.ambient;
        shaderCube["pLight.diffuse"] = sPLight.diffuse;
        shaderCube["pLight.specular"] = sPLight.specular;
        shaderCube["pLight.constCoeff"] = sPLight.constCoeff;
        shaderCube["pLight.linCoeff"] = sPLight.linCoeff;
        shaderCube["pLight.quadCoeff"] = sPLight.quadCoeff;
            
        sSPLight.position = sCamera.pos();
        sSPLight.direction = sCamera.front();
        shaderCube["spLight.position"] = sSPLight.position;
        shaderCube["spLight.direction"] = sSPLight.direction;
        shaderCube["spLight.ambient"] = sSPLight.ambient;
        shaderCube["spLight.diffuse"] = sSPLight.diffuse;
        shaderCube["spLight.specular"] = sSPLight.specular;
        shaderCube["spLight.constCoeff"] = sSPLight.constCoeff;
        shaderCube["spLight.linCoeff"] = sSPLight.linCoeff;
        shaderCube["spLight.quadCoeff"] = sSPLight.quadCoeff;
        shaderCube["spLight.cutOff"] = glm::cos(sSPLight.cutOff);
        shaderCube["spLight.outerCutOff"] = glm::cos(sSPLight.outerCutOff);

        glBindVertexArray(VAO);
        for (int i = 0; i < 1; ++i)
        {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            //model = glm::rotate(model, glm::radians(angle) + curTime, glm::vec3(1.0f, 0.1f * i, 0.5f));
            shaderCube["model"] = model;
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);



        renderTex[1].bind();
        glViewport(0, 0, sWinWidth, sWinHeight);
        // Rendering
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = sCamera2.view();
        projection = sCamera2.projection();

        shaderCube.use();
        shaderCube["view"] = view;
        shaderCube["projection"] = projection;
        shaderCube["viewerPos"] = sCamera2.pos();

        shaderCube["material.diffuse"] = 0;
        crateTex.active(GL_TEXTURE0);
        shaderCube["material.specular"] = 1;
        crateSpecTex.active(GL_TEXTURE1);
        shaderCube["material.shininess"] = sCube.shininess;

        shaderCube["dirLight.direction"] = sDirLight.direction;
        shaderCube["dirLight.ambient"] = sDirLight.ambient;
        shaderCube["dirLight.diffuse"] = sDirLight.diffuse;
        shaderCube["dirLight.specular"] = sDirLight.specular;

        shaderCube["pLight.position"] = sPLight.position;
        shaderCube["pLight.ambient"] = sPLight.ambient;
        shaderCube["pLight.diffuse"] = sPLight.diffuse;
        shaderCube["pLight.specular"] = sPLight.specular;
        shaderCube["pLight.constCoeff"] = sPLight.constCoeff;
        shaderCube["pLight.linCoeff"] = sPLight.linCoeff;
        shaderCube["pLight.quadCoeff"] = sPLight.quadCoeff;

        sSPLight.position = sCamera2.pos();
        sSPLight.direction = sCamera2.front();
        shaderCube["spLight.position"] = sSPLight.position;
        shaderCube["spLight.direction"] = sSPLight.direction;
        shaderCube["spLight.ambient"] = sSPLight.ambient;
        shaderCube["spLight.diffuse"] = sSPLight.diffuse;
        shaderCube["spLight.specular"] = sSPLight.specular;
        shaderCube["spLight.constCoeff"] = sSPLight.constCoeff;
        shaderCube["spLight.linCoeff"] = sSPLight.linCoeff;
        shaderCube["spLight.quadCoeff"] = sSPLight.quadCoeff;
        shaderCube["spLight.cutOff"] = glm::cos(sSPLight.cutOff);
        shaderCube["spLight.outerCutOff"] = glm::cos(sSPLight.outerCutOff);

        shaderCube["model"] = glm::mat4x4();

        glBindVertexArray(VAO);
        for (int i = 0; i < 1; ++i)
        {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            //model = glm::rotate(model, glm::radians(angle) + curTime, glm::vec3(1.0f, 0.1f * i, 0.5f));
            shaderCube["model"] = model;
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);



            // TO SCREEN
        renderTex[0].unbind();
        glViewport(0, 0, sWinWidth, sWinHeight);
        glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        simpleProg.use();
        renderTex[0].activeTex(GL_TEXTURE0);
        renderTex[1].activeTex(GL_TEXTURE1);
        simpleProg["renderTex0"] = 0;
        simpleProg["renderTex1"] = 1;
        simpleProg["iResolution"] = glm::vec2(sWinWidth, sWinHeight);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glDisableVertexAttribArray(0);

        //TwDraw();

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
    else if ((key == GLFW_KEY_C && action == GLFW_PRESS))
        sUseCam1 = !sUseCam1;
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
    sCamera2.fov(sCamera.fov());
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
    //if (sMouseVisible)
    //    return;
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
    glm::vec3 const minCf = glm::rotate(-sCamera.front() * sCamDist, sCamAngle, sCamera.up());
    sCamera2.front(-minCf);
    sCamera2.pos(sCamera.pos() + sCamera.front() * sCamDist + minCf);
}
