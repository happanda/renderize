#pragma once
#include <string>


struct SoilImage
{
    SoilImage();
    ~SoilImage();
    SoilImage(SoilImage const&) = delete;
    SoilImage const& operator=(SoilImage const&) = delete;

    bool load(std::string const& path);
    void clear();
    void* data() const;

    int width() const;
    int height() const;

private:
    unsigned char* mImgBuf;
    int mWidth;
    int mHeight;
};
