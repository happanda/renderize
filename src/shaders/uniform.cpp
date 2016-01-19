#include "uniform.h"
#include "program.h"


Uniform::Uniform(Program const& prog, std::string const& name)
    : mProg(prog)
    , mLoc(glGetUniformLocation(mProg, name.c_str()))
{
}

Uniform::operator GLint() const
{
    return mLoc;
}
