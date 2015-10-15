#include "uniform.h"
#include "program.h"


uniform::uniform(program const& prog, std::string const& name)
    : mProg(prog)
    , mLoc(glGetUniformLocation(mProg, name.c_str()))
{
}

uniform::operator GLint() const
{
    return mLoc;
}
