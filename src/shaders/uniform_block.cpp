#include "uniform_block.h"


UniformBlock::UniformBlock(GLuint bufObject, GLintptr offset)
    : mBO(bufObject)
    , mOffset(offset)
{
}

void UniformBlock::bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, mBO);
}

void UniformBlock::unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
