#include <string>
#include "model.h"
#include "shaders/program.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLint> indices, std::vector<Texture> textures)
    : vertices(std::move(vertices))
    , indices(std::move(indices))
    , textures(std::move(textures))
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::draw(Program const& prog)
{
    GLuint normTexN = 1;
    GLuint diffTexN = 1;
    GLuint specTexN = 1;

    for (GLuint i = 0; i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string paramName = "mat.";
        switch (textures[i].type())
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
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
