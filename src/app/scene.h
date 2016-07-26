#pragma once
#include "data/light.h"
#include "data/mesh.h"
#include "data/mesh_sorter.h"
#include "data/skybox.h"
#include "shaders/program.h"
#include "shaders/uniform_buffer.h"


struct Camera;
struct Model;

struct Scene
{
    template <class Number>
    Scene(glm::tvec2<Number> const& size)
        : mUniBuf("camera")
    {
    }

    void init();

    template <class Number>
    void resize(glm::tvec2<Number> const& size)
    {
        resize(glm::vec2(static_cast<float>(size.x), static_cast<float>(size.y)));
    }

    void update(float dt);
    void draw(Camera& camera, glm::vec4 const& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    void add(DirLight light);
    void add(PointLight light);
    void add(SpotLight light);

private:
    void resize(glm::vec2 const& size);
        
    Mesh mQuadmesh;
    MeshSorter mMeshSorter;

    std::list<DirLight> mDirLights;
    std::list<PointLight> mPointLights;
    std::list<SpotLight> mSpotLights;

    UniformBuffer mUniBuf;

    Program mProg;
    Program mProgInstanced;
    Program mNormalShowProg;
    Program mReflectProg;

    std::shared_ptr<Model> mModel;
    Mesh mCubemesh;
    
    std::shared_ptr<Model> mPlanet;
    std::shared_ptr<Model> mAsteroid;
    std::vector<glm::mat4> mAstPoss;

    Skybox mSkybox;
};
