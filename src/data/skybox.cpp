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

    auto vertices = sSkyboxVertices;
    std::for_each(vertices.begin(), vertices.end(), [](GLfloat& num){ num *= 100.0f; });

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
}

void Skybox::draw(Camera const& camera)
{
    GLboolean priorDepthMask = GL_TRUE;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &priorDepthMask);

    glDepthMask(GL_FALSE);
    mProg.use();
    camera.assign(mProg);
    glBindVertexArray(mVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTex);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(priorDepthMask);
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