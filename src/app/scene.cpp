#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "scene.h"
#include "camera/camera.h"
#include "data/cube.h"
#include "data/model.h"
#include "data/quad.h"
#include "util/checked_call.h"
#include "util/soil_image.h"

int const sAsteroidCount = 1000;

void Scene::init()
{
    DirLight dl = DirLight()
        .direction({ 0.0f, -1.0f, 0.0f })
        .ambient({ 0.6f, 0.6f, 0.6f })
        .diffuse({ 0.1f, 0.1f, 0.7f })
        .specular({ 0.5f, 0.5f, 0.5f });
    mDirLights.emplace_back(dl);
    dl = DirLight()
        .direction({ 0.0f, 1.0f, 0.0f })
        .ambient({ 0.6f, 0.6f, 0.6f })
        .diffuse({ 0.9f, 0.1f, 0.1f })
        .specular({ 0.5f, 0.5f, 0.5f });
    mDirLights.emplace_back(dl);

    PointLight pl = PointLight()
        .position({ 0.0f, 5.0f, 0.0f })
        .ambient({ 0.0f, 0.0f, 0.0f })
        .diffuse({ 0.7f, 0.7f, 0.7f })
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
    mUniBuf.init(mProg, { "projection", "view", "viewerPos" });

    mProgInstanced = createProgram("shaders/simple_instanced.vert", "shaders/simple.frag");
    CHECK(mProgInstanced, "Error creating shader program", return;);

    mNormalShowProg = createProgram("shaders/normal_show.vert", "shaders/normal_show.geom", "shaders/normal_show.frag");
    CHECK(mNormalShowProg, "Error creating shader program", return;);

    //mReflectProg = createProgram("shaders/world_mapped.vert", "shaders/world_mapped.frag");
    //CHECK(mReflectProg, "Error creating shader program", return;);

    /// NANOSUIT
    mModel.reset(new Model);
    mModel->load("nanosuit/nanosuit.obj");
    mModel->noBlending();
    mModel->culling(GL_BACK);

    /// PLANET AND ASTEROIDS
    mPlanet.reset(new Model);
    mPlanet->load("planet/planet.obj");
    mPlanet->noBlending();
    mPlanet->culling(GL_BACK);
    mAsteroid.reset(new Model);
    mAsteroid->load("asteroid/rock.obj");
    mAsteroid->noBlending();
    mAsteroid->culling(GL_BACK);
    
    mAstPoss.resize(sAsteroidCount);
    float const radius = 30.0f;
    float const offset = 2.5f;
    for (int i = 0; i < sAsteroidCount; ++i)
    {
        glm::mat4& mat = mAstPoss[i];
        float angle = static_cast<float>(i) / sAsteroidCount * 360.0f;
        float disp = (std::rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
        float x = std::sin(angle) * radius + disp;
        disp = (std::rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
        float y = disp * 0.4f;
        disp = (std::rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
        float z = std::cos(angle) * radius + disp;

        mat = glm::translate(mat, glm::vec3(x, y, z));

        float scale = (std::rand() % 20) / 100.0f + 0.05f;
        mat = glm::scale(mat, glm::vec3(scale));

        float rot = static_cast<float>(std::rand() % 360);
        mat = glm::rotate(mat, rot, glm::vec3(0.4f, 0.6f, 0.8f));
    }
    mAsteroid->addInstancedModel(mAstPoss);


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
    //mMeshSorter.addMesh(quad1pos, &mQuadmesh);
    //mMeshSorter.addMesh(quad2pos, &mQuadmesh);
    //mMeshSorter.addMesh(quad3pos, &mQuadmesh);
    //mMeshSorter.addMesh(quad4pos, &mQuadmesh);

    SoilCubemapImage imgs;
    imgs[0].load("../tex/skybox/right.jpg");
    imgs[1].load("../tex/skybox/left.jpg");
    imgs[2].load("../tex/skybox/top.jpg");
    imgs[3].load("../tex/skybox/bottom.jpg");
    imgs[4].load("../tex/skybox/back.jpg");
    imgs[5].load("../tex/skybox/front.jpg");
    mSkybox.create(imgs);

    glLineWidth(2.0f);
    glPointSize(2.0f);
    //glEnable(GL_MULTISAMPLE);
}

void Scene::update(float dt)
{
}

void Scene::draw(Camera& camera, glm::vec4 const& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    camera.assign(mUniBuf);

    if (!mMeshSorter.meshes().empty())
        mSkybox.drawFirst(camera);

    mProg.use();
    mUniBuf.bind(mProg);

    mSkybox.tex().active(mProg, "skyboxTexture", 0);
    mProg["DirLightOn"] = true;
    //mProg["model"] = glm::mat4();
    //mCubemesh.draw(mProg);
    //mModel->draw(mProg);

    auto scaleMat = glm::scale(glm::mat4(), glm::vec3(3.0f));
    auto transVec = glm::vec3(0.0f, -1.0f, 0.0f);
    mProg["model"] = glm::translate(glm::rotate(scaleMat, static_cast<float>(glfwGetTime()) / 25.0f, glm::vec3(0.0f, 1.0f, 0.0f)), transVec);
    mPlanet->draw(mProg);

    mProgInstanced.use();
    mUniBuf.bind(mProgInstanced);
    mSkybox.tex().active(mProgInstanced, "skyboxTexture", 0);
    mProgInstanced["DirLightOn"] = true;
    mProgInstanced["model"] = glm::rotate(glm::mat4(), static_cast<float>(glfwGetTime()) / 25.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    mAsteroid->draw(mProgInstanced);

    if (false)
    {
        mNormalShowProg.use();
        mNormalShowProg["uColor"] = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
        mNormalShowProg["NormalMagnitude"] = 0.4f;
        mNormalShowProg["model"] = glm::translate(scaleMat, transVec);
        mUniBuf.bind(mNormalShowProg);
        mCubemesh.draw(mNormalShowProg);
        mModel->draw(mNormalShowProg);
    }

    //mUniBuf.bind(mReflectProg);
    //mReflectProg["model"] = glm::translate(scaleMat, transVec);
    //mSkybox.tex().active(mReflectProg, "skyboxTexture", 0);
    //mReflectProg["reflectOrRefract"] = false;
    //mReflectProg["refractRatio"] = 1.0f / 2.5f;
    //mModel->draw(mProg);

    int idx = 0;
    for (auto& lgh : mDirLights)
    {
        lgh.assign(mProg, "dirLight[" + std::to_string(idx++) + "]");
        lgh.assign(mProgInstanced, "dirLight[" + std::to_string(idx++) + "]");
    }
    for (auto& lgh : mPointLights)
    {
        lgh.assign(mProg, "pLight[" + std::to_string(idx++) + "]");
        lgh.assign(mProgInstanced, "pLight[" + std::to_string(idx++) + "]");
    }
    idx = 0;
    for (auto& lgh : mSpotLights)
    {
        lgh.position(camera.pos());
        lgh.direction(camera.front());
        lgh.assign(mProg, "spLight[" + std::to_string(idx++) + "]");
        lgh.assign(mProgInstanced, "spLight[" + std::to_string(idx++) + "]");
    }

    mProg["PointLightOn"] = false;
    mProg["SpotLightOn"] = false;
    mProgInstanced["PointLightOn"] = false;
    mProgInstanced["SpotLightOn"] = false;
    mMeshSorter.sort(camera.pos());
    for (auto const& mesh : mMeshSorter.meshes())
    {
        mProg["model"] = glm::translate(scaleMat, mesh.first);
        mesh.second->draw(mProg);
    }
    
    if (mMeshSorter.meshes().empty())
        mSkybox.drawLast(camera);
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
}

