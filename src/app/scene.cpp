#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "scene.h"
#include "data/cube.h"
#include "data/model.h"
#include "data/quad.h"
#include "util/checked_call.h"
#include "util/soil_image.h"


void Scene::update(float dt)
{
}

void Scene::draw()
{
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    mProg.use();

    auto scaleMat = glm::scale(glm::mat4(), glm::vec3(1.0f));
    auto transVec = glm::vec3(0.0f, 0.0f, 0.0f);
    mProg["model"] = glm::translate(scaleMat, transVec);

    mCamera.assign(mProg);

    int idx = 0;
    for (auto& lgh : mDirLights)
    {
        lgh->assign(mProg, "dirLight[" + std::to_string(idx++) + "]");
    }
    for (auto& lgh : mPointLights)
    {
        lgh->assign(mProg, "pLight[" + std::to_string(idx++) + "]");
    }
    idx = 0;
    for (auto& lgh : mSpotLights)
    {
        SpotLight* sl = static_cast<SpotLight*>(lgh.get());
        sl->position(mCamera.pos());
        sl->direction(mCamera.front());
        lgh->assign(mProg, "spLight[" + std::to_string(idx++) + "]");
    }

    mProg["SpotLightOn"] = false;// mSpotLightOn;

    //model.draw(mProg);
    mCubemesh.draw(mProg);

    mMeshSorter.sort(mCamera.pos());
    for (auto const& mesh : mMeshSorter.meshes())
    {
        mProg["model"] = glm::translate(scaleMat, mesh.first);
        mesh.second->draw(mProg);
    }

}

void Scene::addDirectional(LightPtr light)
{
    mDirLights.emplace_back(light);
}

void Scene::addPoint(LightPtr light)
{
    mPointLights.emplace_back(light);
}

void Scene::addSpot(LightPtr light)
{
    mSpotLights.emplace_back(light);
}

void Scene::init()
{
    mProg = createProgram("shaders/simple.vert", "shaders/simple.frag");
    CHECK(mProg, "Error creating shader program", return;);

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
    mCubemesh = cubeMesh(crateTexs);
    mCubemesh.noBlending();
    mCubemesh.culling(GL_BACK);

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
    mMeshSorter.addMesh(quad1pos, &quadmesh);
    mMeshSorter.addMesh(quad2pos, &quadmesh);
    mMeshSorter.addMesh(quad3pos, &quadmesh);
    mMeshSorter.addMesh(quad4pos, &quadmesh);
}

void Scene::resize(glm::vec2 const& sz)
{
    mCamera.size(sz);
}

