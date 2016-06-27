#include <string>
#include "mesh.h"
#include "shaders/program.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<TexturePtr> textures)
    : mVertices(std::move(vertices))
    , mIndices(std::move(indices))
    , mTextures(std::move(textures))
    , mVAO(0)
    , mVBO(0)
    , mEBO(0)
{
    initMesh();
}

Mesh::~Mesh()
{
    if (mEBO)
        glDeleteBuffers(1, &mEBO);
    if (mVBO)
        glDeleteBuffers(1, &mVBO);
    if (mVAO)
        glDeleteVertexArrays(1, &mVAO);
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

    //prog.use();
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
        }
        prog[paramName] = i;
        mTextures[i]->active(GL_TEXTURE0 + i);
    }

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

    if (mCulling)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(mCullMode);
        glFrontFace(GL_CCW);
    }
    else
        glDisable(GL_CULL_FACE);

    glBindVertexArray(mVAO);
    if (mIndices.empty())
        glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    else
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    for (GLint i = 0; i < static_cast<GLint>(mTextures.size()); ++i)
    {
        mTextures[i]->unactive(GL_TEXTURE0 + i);
    }
}

Mesh::Mesh(Mesh&& rhs)
    : mVAO(rhs.mVAO)
    , mVBO(rhs.mVBO)
    , mEBO(rhs.mEBO)
    , mVertices(std::move(rhs.mVertices))
    , mIndices(std::move(rhs.mIndices))
    , mTextures(std::move(rhs.mTextures))
{
    rhs.mVAO = rhs.mVBO = rhs.mEBO = 0;
}

Mesh const& Mesh::operator=(Mesh&& rhs)
{
    mVAO = rhs.mVAO;
    mVBO = rhs.mVBO;
    mEBO = rhs.mEBO;
    mVertices = std::move(rhs.mVertices);
    mIndices = std::move(rhs.mIndices);
    mTextures = std::move(rhs.mTextures);
    rhs.mVAO = rhs.mVBO = rhs.mEBO = 0;
    return *this;
}
