#pragma once
#include <list>
#include <vector>
//#include "data/mesh.h"


struct Mesh;

struct MeshSorter
{
    typedef std::pair<glm::vec3, Mesh*> PosMesh;

    MeshSorter();
    ~MeshSorter();

    void addMesh(glm::vec3 const& pos, Mesh* mesh);
    void sort(glm::vec3 const& pos);
    std::vector<PosMesh> const& meshes() const;

    void clear();

private:
    std::vector<PosMesh> mMeshes;
};
