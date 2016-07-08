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
        DirLight dl = DirLight()
            .direction({ 1.0f, 1.0f, -1.0f })
            .ambient({ 0.4f, 0.4f, 0.4f })
            .diffuse({ 0.8f, 0.8f, 0.8f })
            .specular({ 0.4f, 0.4f, 0.4f });
        mDirLights.emplace_back(dl);
                
        PointLight pl = PointLight()
            .position({ -1.0f, -1.0f, 0.0f })
            .ambient({ 0.1f, 0.1f, 0.1f })
            .diffuse({ 0.3f, 0.02f, 0.02f })
            .specular({ 0.5f, 0.1f, 0.1f })
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
    }

    template <class Number>
    void resize(glm::tvec2<Number> const& size)
    {
        resize(glm::vec2(static_cast<float>(size.x), static_cast<float>(size.y)));
    }

    void update(float dt);
    void draw();

    void add(DirLight light);
    void add(PointLight light);
    void add(SpotLight light);

private:
    void init();

    void resize(glm::vec2 const& size);

    Camera mCamera;
    MainCameraUpdater mCamUpdater;
    
    MeshSorter mMeshSorter;

    std::list<DirLight> mDirLights;
    std::list<PointLight> mPointLights;
    std::list<SpotLight> mSpotLights;

    Program mProg;

    Mesh mCubemesh;
};
