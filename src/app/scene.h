#pragma once
#include "camera/camera.h"
#include "camera/camera_updater.h"
#include "data/light.h"
#include "data/mesh.h"
#include "data/mesh_sorter.h"
#include "shaders/program.h"


struct Scene
{
    template <class Number>
    Scene(glm::tvec2<Number> const& size)
        : mCamera(size)
        , mCamUpdater(mCamera)

    {
        DirLight* dl = new DirLight;
        mDirLights.emplace_back(dl);
        dl->direction({ 1.0f, 1.0f, -1.0f })
            .ambient({ 0.4f, 0.4f, 0.4f })
            .diffuse({ 0.8f, 0.8f, 0.8f })
            .specular({ 0.4f, 0.4f, 0.4f });

        PointLight* pl = new PointLight;
        mPointLights.emplace_back(pl);
        pl->position({ -1.0f, -1.0f, 0.0f })
            .ambient({ 0.1f, 0.1f, 0.1f })
            .diffuse({ 0.3f, 0.02f, 0.02f })
            .specular({ 0.5f, 0.1f, 0.1f })
            .constCoeff(1.0f)
            .linCoeff(0.09f)
            .quadCoeff(0.05f);

        SpotLight* sl = new SpotLight;
        mSpotLights.emplace_back(sl);
        sl->ambient({ 0.1f, 0.1f, 0.1f })
            .diffuse({ 0.1f, 0.9f, 0.1f })
            .specular({ 0.5f, 0.5f, 0.5f })
            .constCoeff(pl->constCoeff())
            .linCoeff(pl->linCoeff())
            .quadCoeff(pl->quadCoeff())
            .cutOff(0.05f)
            .outerCutOff(0.2f);
    }

    template <class Number>
    void resize(glm::tvec2<Number> const& size)
    {
        resize(glm::vec2(static_cast<float>(size.x), static_cast<float>(size.y)));
    }

    void update(float dt);
    void draw();

    void addDirectional(LightPtr light);
    void addPoint(LightPtr light);
    void addSpot(LightPtr light);

private:
    void init();

    void resize(glm::vec2 const& size);

    Camera mCamera;
    MainCameraUpdater mCamUpdater;
    
    MeshSorter mMeshSorter;

    std::list<LightPtr> mDirLights;
    std::list<LightPtr> mPointLights;
    std::list<LightPtr> mSpotLights;

    Program mProg;

    Mesh mCubemesh;
};
