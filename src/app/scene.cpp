#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "scene.h"
#include "data/cube.h"
#include "data/model.h"
#include "data/quad.h"
#include "util/checked_call.h"
#include "util/soil_image.h"


void Scene::init()
{
    mCamUpdater.reset(new MainCameraUpdater(mCamera));

    DirLight dl = DirLight()
        .direction({ 1.0f, 1.0f, 1.0f })
        .ambient({ 0.4f, 0.4f, 0.4f })
        .diffuse({ 0.1f, 0.5f, 0.1f })
        .specular({ 0.4f, 0.4f, 0.4f });
    mDirLights.emplace_back(dl);

    PointLight pl = PointLight()
        .position({ 0.0f, 5.0f, 0.0f })
        .ambient({ 0.1f, 0.1f, 0.1f })
        .diffuse({ 0.5f, 0.5f, 0.5f })
        .specular({ 0.1f, 0.1f, 0.7f })
        .constCoeff(1.0f)
        .linCoeff(0.09f)
        .quadCoeff(0.05f);
    mPointLights.emplace_back(pl);

    SpotLight sl = SpotLight()
        .ambient({ 0.1f, 0.1f, 0.1f })
        .diffuse({ 0.1f, 0.9f, 0.1f })
        .specular({ 0.5f, 0.5f, 0.5f })
        .constCoeff(pl.constCoeff())
        .linCoeff(pl.linCoeff())
        .quadCoeff(pl.quadCoeff())
        .cutOff(0.05f)
        .outerCutOff(0.2f);
    mSpotLights.emplace_back(sl);

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
    mQuadmesh = quadMesh(quadTexs);
    //mQuadmesh.noBlending();
    mQuadmesh.blending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mQuadmesh.noCulling();

    glm::vec3 const quad1pos(0.0f, 0.0f, 2.0f);
    glm::vec3 const quad2pos(0.0f, 0.0f, 3.0f);
    glm::vec3 const quad3pos(0.0f, 0.0f, -2.0f);
    glm::vec3 const quad4pos(0.0f, 0.0f, -3.0f);
    mMeshSorter.addMesh(quad1pos, &mQuadmesh);
    mMeshSorter.addMesh(quad2pos, &mQuadmesh);
    mMeshSorter.addMesh(quad3pos, &mQuadmesh);
    mMeshSorter.addMesh(quad4pos, &mQuadmesh);
}

void Scene::update(float dt)
{
    mCamUpdater->update(dt);
}

void Scene::draw()
{
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    mProg.use();
    mProg["DirLightOn"] = true;

    auto scaleMat = glm::scale(glm::mat4(), glm::vec3(1.0f));
    auto transVec = glm::vec3(0.0f, 0.0f, 0.0f);
    mProg["model"] = glm::translate(scaleMat, transVec);

    mCamera.assign(mProg);

    int idx = 0;
    for (auto& lgh : mDirLights)
    {
        lgh.assign(mProg, "dirLight[" + std::to_string(idx++) + "]");
    }
    for (auto& lgh : mPointLights)
    {
        lgh.assign(mProg, "pLight[" + std::to_string(idx++) + "]");
    }
    idx = 0;
    for (auto& lgh : mSpotLights)
    {
        lgh.position(mCamera.pos());
        lgh.direction(mCamera.front());
        lgh.assign(mProg, "spLight[" + std::to_string(idx++) + "]");
    }

    mProg["PointLightOn"] = true;
    mProg["SpotLightOn"] = false;// mSpotLightOn;

    //model.draw(mProg);
    mCubemesh.draw(mProg);

    mProg["PointLightOn"] = false;
    mProg["SpotLightOn"] = false;
    mMeshSorter.sort(mCamera.pos());
    for (auto const& mesh : mMeshSorter.meshes())
    {
        mProg["model"] = glm::translate(scaleMat, mesh.first);
        mesh.second->draw(mProg);
    }

}

void Scene::add(DirLight light)
{
    mDirLights.emplace_back(light);
}

void Scene::add(PointLight light)
{
    mPointLights.emplace_back(light);
}

void Scene::add(SpotLight light)
{
    mSpotLights.emplace_back(light);
}

void Scene::resize(glm::vec2 const& sz)
{
    mCamera.size(sz);
}

