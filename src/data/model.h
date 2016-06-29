#pragma once
#include <map>
#include <assimp/material.h>
#include "data/mesh.h"


struct aiMesh;
struct aiNode;
struct aiScene;


struct Model
{
    Model(std::string const& path);

    Model(Model&& rhs);
    Model const& operator=(Model&& rhs);
    Model(Model const&) = delete;
    Model const& operator=(Model const&) = delete;

    void noBlending();
    void blending(GLenum sfactor, GLenum dfactor);
    void blending(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

    void noCulling();
    void culling(GLenum mode);

    void draw(Program const& prog) const;

private:
    void loadModel(std::string const& path);
    void processNode(aiNode* node, aiScene const* scene);
    Mesh processMesh(aiMesh* mesh, aiScene const* scene);

    std::vector<TexturePtr> loadMaterialTextures(aiMaterial* mat, aiTextureType aiType, TexType type);

    std::vector<Mesh> mMeshes;
    std::string mDir;

    std::map<std::string, TexturePtr> mLoadedTextures;
};