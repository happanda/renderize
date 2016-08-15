#pragma once
#include <string>


    /// A class to concatenate files' contents
struct Cat
{
    bool cat(std::string const& path, bool withNewLine = false);
    void clear();

    operator std::string const&() const;

private:
    std::string mText;
};

std::string readAllText(std::string const& path);
