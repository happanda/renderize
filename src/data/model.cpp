#include <string>
#include <utility>
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "model.h"
#include "shaders/program.h"
#include "util/soil_image.h"

using std::swap;


Model::Model()
{
}

Model::~Model()
{
    free();
}

Model::Model(Model&& rhs)
{
    free();
    swap(mMeshes, rhs.mMeshes);
    swap(mDir, rhs.mDir);
    swap(mLoadedTextures, rhs.mLoadedTextures);
}

Model const& Model::operator=(Model&& rhs)
{
    free();
    swap(mMeshes, rhs.mMeshes);
    swap(mDir, rhs.mDir);
    swap(mLoadedTextures, rhs.mLoadedTextures);
    return *this;
}

void Model::noBlending()
{
    for (auto& mesh : mMeshes)
        mesh.noBlending();
}

void Model::blending(GLenum sfactor, GLenum dfactor)
{
    for (auto& mesh : mMeshes)
        mesh.blending(sfactor, dfactor);
}

void Model::blending(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    for (auto& mesh : mMeshes)
        mesh.blending(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}

void Model::noCulling()
{
    for (auto& mesh : mMeshes)
        mesh.noCulling();
}

void Model::culling(GLenum mode)
{
    for (auto& mesh : mMeshes)
        mesh.culling(mode);
}

void Model::load(std::string const& path)
{
    Assimp::Importer importer;
    aiScene const* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) == AI_SCENE_FLAGS_INCOMPLETE)
        || !scene->mRootNode)
    {
        //std::cerr << "ERROR loading model with Assimp: " << importer.GetErrorString() << std::endl;
        return;
    }
    mDir = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::free()
{
    mMeshes.clear();
    mDir.clear();
    mLoadedTextures.clear();
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
    std::vector<TexturePtr> textures;
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
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
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
        std::vector<TexturePtr> diffMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TexType::Diffuse);
        textures.insert(textures.cend(), diffMaps.cbegin(), diffMaps.cend());

        std::vector<TexturePtr> specMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TexType::Specular);
        textures.insert(textures.cend(), specMaps.cbegin(), specMaps.cend());

        // TODO: AMBIENT is used cause Assimp doesn't work properly with reflective?
        std::vector<TexturePtr> reflMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TexType::Reflection);
        textures.insert(textures.cend(), reflMaps.cbegin(), reflMaps.cend());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<TexturePtr> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType aiType, TexType type)
{
    std::vector<TexturePtr> textures;
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
            TexturePtr tex(new Texture);
            SoilImage soilImage;
            soilImage.load(mDir + "/" + texPath);
            tex->create(soilImage);
            tex->genMipMap();
            tex->setType(type);
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
