#include <iostream>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

#include "app.h"
#include "buffers/frame_buffer.h"
#include "buffers/render_buffer.h"
#include "buffers/texture.h"
#include "data/cube.h"
#include "data/light.h"
#include "data/mesh_sorter.h"
#include "data/model.h"
#include "data/quad.h"
#include "input/mouse.h"
#include "shaders/program.h"
#include "util/checked_call.h"
#include "util/date.h"
#include "util/soil_image.h"

glm::mat3x3 sKernel;

namespace
{
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

    void charCallback(GLFWwindow* window, unsigned int symb)
    {
    }

    void windowSizeCallback(GLFWwindow* window, int sizeX, int sizeY)
    {
        APP().resize(glm::ivec2(sizeX, sizeY));
    }
} // anonymous namespace


App::App()
    : mWinSize(800, 800)
    , mWindow(nullptr)
    , mKeys(GLFW_KEY_LAST, false)
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
    
    // Initialize some GLFW callbacks
    glfwSetInputMode(mWindow, GLFW_CURSOR, mMouseVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(mWindow, keyCallback);
    Mouse::create(mWindow);
    //glfwSetCursorPosCallback(mWindow, mouseCallback);
    //glfwSetScrollCallback(mWindow, scrollCallback);
    //glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
    glfwSetCharCallback(mWindow, charCallback);
    glfwSetWindowSizeCallback(mWindow, windowSizeCallback);
    return true;
}

bool App::shouldClose() const
{
    Mouse::destroy();
    return glfwWindowShouldClose(mWindow) != 0;
}

void App::resize(glm::ivec2 const& size)
{
    mWinSize = size;
    glViewport(0, 0, mWinSize.x, mWinSize.y);
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
    else if (key == GLFW_KEY_F)
    {
        mSpotLightOn = !mSpotLightOn;
    }
    else if (key == GLFW_KEY_R)
    {
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
    MeshSorter meshSorter;

    DirLight dirLight = DirLight()
        .direction({ 1.0f, 1.0f, -1.0f })
        .ambient({ 0.4f, 0.4f, 0.4f })
        .diffuse({ 0.8f, 0.8f, 0.8f })
        .specular({ 0.4f, 0.4f, 0.4f });

    PointLight pLight = PointLight()
        .position({ -1.0f, -1.0f, 0.0f })
        .ambient({ 0.1f, 0.1f, 0.1f })
        .diffuse({ 0.3f, 0.02f, 0.02f })
        .specular({ 0.5f, 0.1f, 0.1f })
        .constCoeff(1.0f)
        .linCoeff(0.09f)
        .quadCoeff(0.05f);

    SpotLight sLight = SpotLight()
        .ambient({ 0.1f, 0.1f, 0.1f })
        .diffuse({ 0.1f, 0.9f, 0.1f })
        .specular({ 0.5f, 0.5f, 0.5f })
        .constCoeff(pLight.mConstCoeff)
        .linCoeff(pLight.mLinCoeff)
        .quadCoeff(pLight.mQuadCoeff)
        .cutOff(0.05f)
        .outerCutOff(0.2f);


    Program prog = createProgram("shaders/simple.vert", "shaders/simple.frag");
    CHECK(prog, "Error creating shader program", return;);

        /// NANOSUIT
    //Model model("nanosuit/nanosuit.obj");
    //model.noBlending();
    //model.culling(GL_BACK);

        /// CUBE
    std::vector<TexturePtr> crateTexs(2);
    crateTexs[0].reset(new Texture);
    crateTexs[1].reset(new Texture);

    SoilImage soilImage;
    CHECK(soilImage.load("../tex/crate.png"), "Error loading crate texture", );
    crateTexs[0]->create(soilImage);
    CHECK(soilImage.load("../tex/crate_specular.png"), "Error loading crate specular texture", );
    crateTexs[1]->create(soilImage);
    crateTexs[0]->genMipMap();
    crateTexs[0]->setType(TexType::Normal);
    crateTexs[1]->genMipMap();
    crateTexs[1]->setType(TexType::Specular);
    Mesh cubemesh = cubeMesh(crateTexs);
    cubemesh.noBlending();
    cubemesh.culling(GL_BACK);

        /// QUAD
    std::vector<TexturePtr> quadTexs(1);
    quadTexs[0].reset(new Texture);
    CHECK(soilImage.load("../tex/transparent_window.png"), "Error loading transparent window texture", );
    quadTexs[0]->create(soilImage);
    quadTexs[0]->genMipMap();
    quadTexs[0]->setType(TexType::Normal);
    Mesh quadmesh = quadMesh(quadTexs);
    //quadmesh.noBlending();
    quadmesh.blending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    quadmesh.noCulling();

    glm::vec3 const quad1pos(0.0f, 0.0f, 2.0f);
    glm::vec3 const quad2pos(0.0f, 0.0f, 3.0f);
    glm::vec3 const quad3pos(0.0f, 0.0f, -4.0f);
    glm::vec3 const quad4pos(0.0f, 0.0f, -5.0f);
    meshSorter.addMesh(quad1pos, &quadmesh);
    meshSorter.addMesh(quad2pos, &quadmesh);
    meshSorter.addMesh(quad3pos, &quadmesh);
    meshSorter.addMesh(quad4pos, &quadmesh);

    GLenum errorCode = glGetError();
    FrameBuffer frameBuffer;
    Texture texture;
    RenderBuffer renderBuffer;
    texture.create(mWinSize.x, mWinSize.y, InternalFormat::Color);
    frameBuffer.attach(texture);
    renderBuffer.create(mWinSize.x, mWinSize.y, InternalFormat::Depth);
    frameBuffer.attach(renderBuffer);
    CHECK(frameBuffer.isComplete(), "Error: FrameBuffer is not complete", );
    frameBuffer.unbind();
    

    Program quadProg = createProgram("shaders/asis.vert", "shaders/post_kernel3x3.frag");
    CHECK(quadProg, "Error creating quad shader program", return;);

    
    GLfloat quadVertices[] = {
        // Positions   // TexCoords
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);


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
        
        //sLight.position(mCamera.pos());
        //sLight.direction(mCamera.front());

        //// Rendering
        //frameBuffer.bind();

        //glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_DEPTH_TEST);

        //prog.use();
        //auto scaleMat = glm::scale(glm::mat4(), glm::vec3(1.0f));
        //auto transVec = glm::vec3(0.0f, 0.0f, 0.0f);
        //prog["model"] = glm::translate(scaleMat, transVec);
        //prog["view"] = mCamera.view();
        //prog["projection"] = mCamera.projection();
        //prog["viewerPos"] = mCamera.pos();

        //prog["dirLight.direction"] = dirLight.mDirection;
        //prog["dirLight.ambient"] = dirLight.mAmbient;
        //prog["dirLight.diffuse"] = dirLight.mDiffuse;
        //prog["dirLight.specular"] = dirLight.mSpecular;
        //prog["pLight.position"] = pLight.mPosition;
        //prog["pLight.ambient"] = pLight.mAmbient;
        //prog["pLight.diffuse"] = pLight.mDiffuse;
        //prog["pLight.specular"] = pLight.mSpecular;
        //prog["pLight.constCoeff"] = pLight.mConstCoeff;
        //prog["pLight.linCoeff"] = pLight.mLinCoeff;
        //prog["pLight.quadCoeff"] = pLight.mQuadCoeff;
        //prog["spLight.position"] = sLight.mPosition;
        //prog["spLight.direction"] = sLight.mDirection;
        //prog["spLight.ambient"] = sLight.mAmbient;
        //prog["spLight.diffuse"] = sLight.mDiffuse;
        //prog["spLight.specular"] = sLight.mSpecular;
        //prog["spLight.constCoeff"] = sLight.mConstCoeff;
        //prog["spLight.linCoeff"] = sLight.mLinCoeff;
        //prog["spLight.quadCoeff"] = sLight.mQuadCoeff;
        //prog["spLight.cutOff"] = glm::cos(sLight.mCutOff);
        //prog["spLight.outerCutOff"] = glm::cos(sLight.mOuterCutOff);

        //prog["SpotLightOn"] = mSpotLightOn;

        ////model.draw(prog);
        //cubemesh.draw(prog);

        //meshSorter.sort(mCamera.pos());
        //for (auto const& mesh : meshSorter.meshes())
        //{
        //    prog["model"] = glm::translate(scaleMat, mesh.first);
        //    mesh.second->draw(prog);
        //}

        //frameBuffer.unbind();
        //
        //// Rendering
        //glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        //quadProg.use();
        //quadProg["offset"] = glm::vec2(1.0f / 400.0f, 1.0f / 400.0f);// glm::vec2(1 / static_cast<float>(mWinSize.x), 1 / static_cast<float>(mWinSize.y));
        ///*quadProg["kernel"] = glm::mat3x3(
        //    -1.0f, -1.0f, -1.0f,
        //    -1.0f,  9.0f, -1.0f,
        //    -1.0f, -1.0f, -1.0f
        //    );*/
        //quadProg["kernel"] = glm::mat3x3(
        //    0.0f, 1.0f, 2.0f,
        //    3.0f, -4.0f, 5.0f,
        //    6.0f, -7.0f, -8.0f
        //    ) / 32.0f;
        //glBindVertexArray(quadVAO);
        //texture.active(GL_TEXTURE0);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glBindVertexArray(0);
        //glDisableVertexAttribArray(0);
        //
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


    Program prog = createProgram("shaders/fragment_demo.vert", "shaders/" + demoName + ".frag");
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
