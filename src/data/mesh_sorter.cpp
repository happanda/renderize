#include <algorithm>
#include <glm/geometric.hpp>
#include "mesh_sorter.h"
#include "data/mesh.h"


MeshSorter::MeshSorter()
{
}

MeshSorter::~MeshSorter()
{
}

void MeshSorter::addMesh(glm::vec3 const& pos, Mesh* mesh)
{
    mMeshes.emplace_back(pos, mesh);
}

void MeshSorter::sort(glm::vec3 const& pos)
{
    std::sort(mMeshes.rbegin(), mMeshes.rend(), [pos](PosMesh const& lhs, PosMesh const& rhs)
    {
        return glm::distance(lhs.first, pos) < glm::distance(rhs.first, pos);
    });
}

std::vector<MeshSorter::PosMesh> const& MeshSorter::meshes() const
{
    return mMeshes;
}

void MeshSorter::clear()
{
    mMeshes.clear();
}
