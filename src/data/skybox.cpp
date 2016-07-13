#include <iostream>
#include <utility>
#include <GL/glew.h>
#include "skybox.h"
#include "camera/camera.h"
#include "data/cube.h"
#include "util/checked_call.h"
#include "util/soil_image.h"

using std::swap;


Skybox::Skybox()
    : mVAO(0)
    , mVBO(0)
{
}

Skybox::~Skybox()
{
    free();
}

Skybox::Skybox(Skybox&& rhs)
    : mTex(std::move(rhs.mTex))
    , mProg(std::move(rhs.mProg))
    , mVAO(0)
    , mVBO(0)
{
    swap(mVAO, rhs.mVAO);
    swap(mVBO, rhs.mVBO);
}

Skybox const& Skybox::operator=(Skybox&& rhs)
{
    mTex = std::move(rhs.mTex);
    mProg = std::move(rhs.mProg);
    swap(mVAO, rhs.mVAO);
    swap(mVBO, rhs.mVBO);
    return *this;
}

void Skybox::create(SoilCubemapImage const& imgs)
{
    mTex.createCubemap(imgs);

    mProg = createProgram("shaders/skybox.vert", "shaders/skybox.frag");
    CHECK(mProg, "Error creating skybox shader program", );

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sSkyboxVertices.size() * sizeof(GLfloat), sSkyboxVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
}

void Skybox::drawFirst(Camera const& camera)
{
    GLboolean priorDepthMask = GL_TRUE;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &priorDepthMask);

    glDepthMask(GL_FALSE);
    mProg.use();
    mProg["postScene"] = false;
    mProg["view"] = glm::mat4(glm::mat3(camera.view()));
    mProg["projection"] = camera.projection();
    mProg["skyboxTexture"] = 0;
    glBindVertexArray(mVAO);
    mTex.active(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    mTex.unactive(GL_TEXTURE0);

    glDepthMask(priorDepthMask);
}

void Skybox::drawLast(Camera const& camera)
{
    GLint priorDepthFunc = GL_LESS;
    glGetIntegerv(GL_DEPTH_FUNC, &priorDepthFunc);

    glDepthFunc(GL_LEQUAL);
    mProg.use();
    mProg["postScene"] = true;
    mProg["view"] = glm::mat4(glm::mat3(camera.view()));
    mProg["projection"] = camera.projection();
    mProg["skyboxTexture"] = 0;
    glBindVertexArray(mVAO);
    mTex.active(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    mTex.unactive(GL_TEXTURE0);

    glDepthFunc(priorDepthFunc);
}

Texture& Skybox::tex()
{
    return mTex;
}

void Skybox::free()
{
    if (mVBO)
        glDeleteBuffers(1, &mVBO);
    if (mVAO)
        glDeleteVertexArrays(1, &mVAO);
    mTex.free();
    mProg.free();
    mVAO = 0;
    mVBO = 0;
}