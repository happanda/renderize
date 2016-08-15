#include <fstream>
#include <sstream>
#include "cat.h"


bool Cat::cat(std::string const& path, bool withNewLine)
{
    if (withNewLine)
        mText.append("\n");
    mText.append(readAllText(path));
    return true;
}

void Cat::clear()
{
    mText.clear();
}

Cat::operator std::string const&() const
{
    return mText;
}


std::string readAllText(std::string const& path)
{
    std::ifstream ifstr(path);
    if (!ifstr)
        return std::string();
    std::stringstream text;
    ifstr >> text.rdbuf();
    return text.str();
}
