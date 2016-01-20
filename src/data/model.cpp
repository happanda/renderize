#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "model.h"
#include "shaders/program.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
    : mVertices(std::move(vertices))
    , mIndices(std::move(indices))
    , mTextures(std::move(textures))
{
    initMesh();
}

void Mesh::initMesh()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::draw(Program const& prog) const
{
    GLuint normTexN = 1;
    GLuint diffTexN = 1;
    GLuint specTexN = 1;

    for (GLuint i = 0; i < mTextures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string paramName = "mat.";
        switch (mTextures[i].type())
        {
        case TexType::Normal:
            paramName.append("texNorm" + std::to_string(normTexN++));
            break;
        case TexType::Diffuse:
            paramName.append("texDiff" + std::to_string(diffTexN++));
            break;
        case TexType::Specular:
            paramName.append("texSpec" + std::to_string(specTexN++));
            break;
        }

        prog[paramName] = i;
        glBindTexture(GL_TEXTURE_2D, mTextures[i]);
    }

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh::Mesh(Mesh&& rhs)
    : mVAO(rhs.mVAO)
    , mVBO(rhs.mVBO)
    , mEBO(rhs.mEBO)
    , mVertices(std::move(rhs.mVertices))
    , mIndices(std::move(rhs.mIndices))
    , mTextures(std::move(rhs.mTextures))
{
}

Mesh const& Mesh::operator=(Mesh&& rhs)
{
    mVAO = rhs.mVAO;
    mVBO = rhs.mVBO;
    mEBO = rhs.mEBO;
    mVertices = std::move(rhs.mVertices);
    mIndices = std::move(rhs.mIndices);
    mTextures = std::move(rhs.mTextures);
    return *this;
}



Model::Model(std::string const& path)
{
    loadModel(path);
}

void Model::loadModel(std::string const& path)
{
    Assimp::Importer importer;
    aiScene const* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == AI_SCENE_FLAGS_INCOMPLETE)
        || !scene->mRootNode)
    {
        std::cerr << "ERROR loading model with Assimp: " << importer.GetErrorString() << std::endl;
        return;
    }
    mDir = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, aiScene const* scene)
{
    for (unsigned i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.emplace_back(processMesh(mesh, scene));
    }
    for (unsigned i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, aiScene const* scene)
{
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<Texture> textures;
    for (GLuint i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;
        if (mesh->mTextureCoords[0])
        {
            vertex.texCoords.x = mesh->mTextureCoords[0]->x;
            vertex.texCoords.y = mesh->mTextureCoords[0]->y;
        }
        vertices.emplace_back(vertex);
    }

    for (GLuint i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace const& face = mesh->mFaces[i];
        for (GLuint k = 0; k < face.mNumIndices; ++k)
            indices.push_back(face.mIndices[k]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TexType::Diffuse);
        textures.insert(textures.cend(), diffMaps.cbegin(), diffMaps.cend());

        std::vector<Texture> specMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TexType::Specular);
        textures.insert(textures.cend(), specMaps.cbegin(), specMaps.cend());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType aiType, TexType type)
{
    std::vector<Texture> textures;
    for (unsigned i = 0; i < mat->GetTextureCount(aiType); ++i)
    {
        aiString str;
        mat->GetTexture(aiType, i, &str);
        std::string texPath = str.C_Str();

        auto texIt = mLoadedTextures.find(texPath);
        if (texIt != mLoadedTextures.end())
            textures.push_back(texIt->second);
        else
        {
            Texture tex;
            tex.load(mDir + texPath);
            tex.setType(type);
            mLoadedTextures[texPath] = tex;
            textures.push_back(tex);
        }
    }
    return textures;
}

void Model::draw(Program const& prog) const
{
    for (auto const& mesh : mMeshes)
        mesh.draw(prog);
}


