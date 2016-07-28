#include <list>
#include <sys/stat.h>
#include <utility>
#include "program_manager.h"
#include "shaders/program.h"

#ifdef _WIN32
#   define STAT_NAME  _stat
#else
#   define STAT_NAME  stat
#endif


namespace std {
    template <>
    struct hash<Program> {
    public:
        size_t operator()(Program const& prog) const
        {
            return prog.hash();
        }
    };
}

std::uint64_t lastModifyTime(std::string const& path)
{
    static struct STAT_NAME st;
    STAT_NAME(path.c_str(), &st);
    return static_cast<uint64_t>(st.st_mtime);
}


struct ShaderFiles
{
    ShaderFiles(std::string vertPath, std::uint64_t vertMod, std::string geomPath, std::uint64_t geomMod, std::string fragPath, std::uint64_t fragMod)
        : vertPath(vertPath)
        , vertMod(vertMod)
        , geomPath(geomPath)
        , geomMod(geomMod)
        , fragPath(fragPath)
        , fragMod(fragMod)
    {
    }

    std::string vertPath;
    std::uint64_t vertMod{ 0 };
    std::string geomPath;
    std::uint64_t geomMod{ 0 };
    std::string fragPath;
    std::uint64_t fragMod{ 0 };
};


struct ProgramManager::impl
{
    impl()
    {
    }

    Program create(std::string const& vertShaderPath, std::string const& fragShaderPath)
    {
        Program prog;
        prog.create(vertShaderPath, fragShaderPath);
        ShaderFiles sf = ShaderFiles(vertShaderPath, lastModifyTime(vertShaderPath), std::string(), static_cast<std::uint64_t>(0), fragShaderPath, lastModifyTime(fragShaderPath));
        mProgs.emplace_back(prog, sf);
        return prog;
    }

    Program create(std::string const& vertShaderPath, std::string const& geomShaderPath, std::string const& fragShaderPath)
    {
        Program prog;
        prog.create(vertShaderPath, geomShaderPath, fragShaderPath);
        ShaderFiles sf{ vertShaderPath, lastModifyTime(vertShaderPath), geomShaderPath, lastModifyTime(geomShaderPath), fragShaderPath, lastModifyTime(fragShaderPath) };
        mProgs.emplace_back(prog, sf);
        return prog;
    }

    void checkChanges()
    {
        for (auto& pr : mProgs)
        {
            ShaderFiles& sf = pr.second;
            bool updated = false;
            if (sf.vertMod != 0 && lastModifyTime(sf.vertPath) != sf.vertMod)
                updated = true;
            else if (sf.geomMod != 0 && lastModifyTime(sf.geomPath) != sf.geomMod)
                updated = true;
            else if (sf.fragMod != 0 && lastModifyTime(sf.fragPath) != sf.fragMod)
                updated = true;

            if (updated)
            {
                if (sf.geomMod == 0)
                    pr.first.create(sf.vertPath, sf.fragPath);
                else
                    pr.first.create(sf.vertPath, sf.geomPath, sf.fragPath);
            }
        }
    }

private:
    std::list<std::pair<Program, ShaderFiles>> mProgs;
};



ProgramManager::ProgramManager()
    : mImpl(new impl)
{
}

ProgramManager::ProgramManager(ProgramManager&& rhs)
    : mImpl(std::move(rhs.mImpl))
{
}

ProgramManager::~ProgramManager()
{
}

ProgramManager const& ProgramManager::operator=(ProgramManager&& rhs)
{
    mImpl = std::move(rhs.mImpl);
    return *this;
}

Program ProgramManager::create(std::string const& vertShaderPath, std::string const& fragShaderPath)
{
    return mImpl->create(vertShaderPath, fragShaderPath);
}

Program ProgramManager::create(std::string const& vertShaderPath, std::string const& geomShaderPath, std::string const& fragShaderPath)
{
    return mImpl->create(vertShaderPath, geomShaderPath, fragShaderPath);
}

void ProgramManager::checkChanges()
{
    mImpl->checkChanges();
}
