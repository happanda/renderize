#pragma once
#include <memory>
#include "shaders/fwd.h"


    /// The struct holds a number of Programs and is able to check for the initial files changes, recompiling the programs
struct ProgramManager
{
    ProgramManager();
    ~ProgramManager();
    ProgramManager(ProgramManager&& rhs);
    ProgramManager const& operator=(ProgramManager&& rhs);

    Program create(std::string const& vertShaderPath, std::string const& fragShaderPath);
    Program create(std::string const& vertShaderPath, std::string const& geomShaderPath, std::string const& fragShaderPath);
    void checkChanges();

private:
    struct impl;
    std::unique_ptr<impl> mImpl;
};
