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
    {
    }

    void init();

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
    void resize(glm::vec2 const& size);

    Camera mCamera;
    std::shared_ptr<MainCameraUpdater> mCamUpdater;
    
    Mesh mQuadmesh;
    MeshSorter mMeshSorter;

    std::list<DirLight> mDirLights;
    std::list<PointLight> mPointLights;
    std::list<SpotLight> mSpotLights;

    Program mProg;

    Mesh mCubemesh;
};
