#include <string>
#include <utility>
#include "mesh.h"
#include "shaders/program.h"

using std::swap;


Mesh::Mesh()
    : mVAO(0)
    , mVBO(0)
    , mEBO(0)
    , mInstVBO(0)
    , mInstCount(0)
{
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<TexturePtr> textures)
    : mVertices(std::move(vertices))
    , mIndices(std::move(indices))
    , mTextures(std::move(textures))
    , mVAO(0)
    , mVBO(0)
    , mEBO(0)
    , mInstVBO(0)
    , mInstCount(0)
{
    initMesh();
}

Mesh::~Mesh()
{
    free();
}

Mesh::Mesh(Mesh&& rhs)
{
    free();
    swap(mVAO, rhs.mVAO);
    swap(mVBO, rhs.mVBO);
    swap(mEBO, rhs.mEBO);
    swap(mInstVBO, rhs.mInstVBO);
    swap(mInstCount, rhs.mInstCount);
    swap(mVertices, rhs.mVertices);
    swap(mIndices, rhs.mIndices);
    swap(mTextures, rhs.mTextures);
    swap(mBlending, rhs.mBlending);
    swap(mSfactorRGB, rhs.mSfactorRGB);
    swap(mDfactorRGB, rhs.mDfactorRGB);
    swap(mSfactorAlpha, rhs.mSfactorAlpha);
    swap(mDfactorAlpha, rhs.mDfactorAlpha);
    swap(mCulling, rhs.mCulling);
    swap(mCullMode, rhs.mCullMode);
}

Mesh const& Mesh::operator=(Mesh&& rhs)
{
    free();
    swap(mVAO, rhs.mVAO);
    swap(mVBO, rhs.mVBO);
    swap(mEBO, rhs.mEBO);
    swap(mInstVBO, rhs.mInstVBO);
    swap(mInstCount, rhs.mInstCount);
    swap(mVertices, rhs.mVertices);
    swap(mIndices, rhs.mIndices);
    swap(mTextures, rhs.mTextures);
    swap(mBlending, rhs.mBlending);
    swap(mSfactorRGB, rhs.mSfactorRGB);
    swap(mDfactorRGB, rhs.mDfactorRGB);
    swap(mSfactorAlpha, rhs.mSfactorAlpha);
    swap(mDfactorAlpha, rhs.mDfactorAlpha);
    swap(mCulling, rhs.mCulling);
    swap(mCullMode, rhs.mCullMode);
    return *this;
}

void Mesh::noBlending()
{
    mBlending = Blending::None;
}

void Mesh::blending(GLenum sfactor, GLenum dfactor)
{
    mBlending = Blending::Simple;
    mSfactorAlpha = sfactor;
    mDfactorAlpha = dfactor;
}

void Mesh::blending(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    mBlending = Blending::Separate;
    mSfactorRGB = sfactorRGB;
    mDfactorRGB = dfactorRGB;
    mSfactorAlpha = sfactorAlpha;
    mDfactorAlpha = dfactorAlpha;
}

void Mesh::noCulling()
{
    mCulling = false;
}

void Mesh::culling(GLenum mode)
{
    mCulling = true;
    mCullMode = mode;
}

void Mesh::addInstancedModel(std::vector<glm::mat4> const& matrices)
{
    mInstCount = matrices.size();
    if (!mInstCount)
        return;
    glBindVertexArray(mVAO);
    glGenBuffers(1, &mInstVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mInstVBO);
    glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);

    GLsizei vec4size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (GLvoid*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (GLvoid*)(vec4size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (GLvoid*)(2 * vec4size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (GLvoid*)(3 * vec4size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void Mesh::free()
{
    if (mEBO)
        glDeleteBuffers(1, &mEBO);
    if (mVBO)
        glDeleteBuffers(1, &mVBO);
    if (mInstVBO)
        glDeleteBuffers(1, &mInstVBO);
    if (mVAO)
        glDeleteVertexArrays(1, &mVAO);
    mInstCount = 0;
    mVertices.clear();
    mIndices.clear();
    mTextures.clear();
    mBlending = Blending::None;
    mSfactorRGB = 0;
    mSfactorAlpha = 0;
    mSfactorAlpha = 0;
    mDfactorAlpha = 0;
    mCulling = false;
    mCullMode = 0;
}

GLuint Mesh::vao() const
{
    return mVAO;
}

GLuint Mesh::vbo() const
{
    return mVBO;
}

GLuint Mesh::ebo() const
{
    return mEBO;
}

std::vector<Vertex> const& Mesh::vertices() const
{
    return mVertices;
}

std::vector<GLuint> const& Mesh::indices() const
{
    return mIndices;
}

void Mesh::initMesh()
{
    bool indexed = !mIndices.empty();

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    if (indexed)
        glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW);

    if (indexed)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);
    }

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
    GLuint reflTexN = 1;

    prog.use();
    bool texReflAssigned = false;
    for (GLint i = 0; i < static_cast<GLint>(mTextures.size()); ++i)
    {
        std::string paramName = "material.";
        switch (mTextures[i]->type())
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
        case TexType::Reflection:
            paramName.append("texRefl" + std::to_string(reflTexN++));
            texReflAssigned = true;
            break;
        }
        prog[paramName] = i;
        mTextures[i]->active(i);
    }
    prog["texReflAssigned"] = texReflAssigned;

    GLboolean priorBlending = GL_FALSE;
    glGetBooleanv(GL_BLEND, &priorBlending);

    if (mBlending == Blending::None)
        glDisable(GL_BLEND);
    else
    {
        glEnable(GL_BLEND);
        if (mBlending == Blending::Simple)
            glBlendFunc(mSfactorAlpha, mDfactorAlpha);
        else
            glBlendFuncSeparate(mSfactorRGB, mDfactorRGB, mSfactorAlpha, mDfactorAlpha);
    }

    GLboolean priorCulling = GL_FALSE;
    glGetBooleanv(GL_CULL_FACE, &priorCulling);

    if (mCulling)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(mCullMode);
        glFrontFace(GL_CCW);
    }
    else
        glDisable(GL_CULL_FACE);

    glBindVertexArray(mVAO);
    if (!mInstCount)
    {
        if (mIndices.empty())
            glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
        else
            glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        if (mIndices.empty())
            glDrawArraysInstanced(GL_TRIANGLES, 0, mVertices.size(), mInstCount);
        else
            glDrawElementsInstanced(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0, mInstCount);
    }
    glBindVertexArray(0);

    for (GLint i = 0; i < static_cast<GLint>(mTextures.size()); ++i)
    {
        mTextures[i]->unactive(i);
    }

    if (priorBlending)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
    if (priorBlending)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}
