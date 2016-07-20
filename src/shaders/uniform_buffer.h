#pragma once
#include <map>
#include <string>
#include <vector>
#include <GL/glew.h>
#include "buffers/fwd.h"
#include "shaders/uniform_block.h"


struct Program;

struct UniformBuffer
{
    UniformBuffer(std::string const& name);
    ~UniformBuffer();
    
    UniformBuffer(UniformBuffer&& rhs);
    UniformBuffer const& operator=(UniformBuffer&& rhs);
    UniformBuffer(UniformBuffer const&) = delete;
    UniformBuffer const& operator=(UniformBuffer const&) = delete;

    bool init(Program const& prog, std::vector<std::string> const& paramNames);

    UniformBlock operator[](std::string const& uniName) const;
    UniformBlock operator[](char const* uniName) const;

    operator GLuint() const;

    void bind() const;
    void unbind() const;

    void bind(Program& prog) const;

private:
    void free();

    std::string mName;
    GLuint mUBO; /// uniform buffer object handler
    GLuint mBP;  /// binding point (increasing linearly with every new UniformBuffer)
    std::map<std::string, int> mOffsets;
};
