#pragma once
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <assimp/material.h>
#include "textures/texture.h"


struct Vertex
{
    glm::vec3  position;
    glm::vec3  normal;
    glm::vec2  texCoords;
};

struct Program;
struct Texture;
struct aiMesh;
struct aiNode;
struct aiScene;


struct Mesh
{
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<TexturePtr> textures);
    void draw(Program const& prog) const;

    Mesh(Mesh&& rhs);
    Mesh const& operator=(Mesh&& rhs);

private:
    void initMesh();

    GLuint mVAO, mVBO, mEBO;

    std::vector<Vertex>  mVertices;
    std::vector<GLuint>  mIndices;
    std::vector<TexturePtr> mTextures;
};


struct Model
{
    Model(std::string const& path);

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
