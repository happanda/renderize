#include <SOIL.h>
#include "soil_image.h"


SoilImage::SoilImage()
    : mImgBuf(nullptr)
    , mWidth(0)
    , mHeight(0)
{
}

SoilImage::~SoilImage()
{
    clear();
}

bool SoilImage::load(std::string const& path)
{
    clear();
    mImgBuf = SOIL_load_image(path.c_str(), &mWidth, &mHeight, 0, SOIL_LOAD_RGBA);
    return mImgBuf != nullptr;
}

void SoilImage::clear()
{
    if (mImgBuf)
    {
        SOIL_free_image_data(mImgBuf);
        mImgBuf = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void* SoilImage::data() const
{
    return static_cast<void*>(mImgBuf);
}

int SoilImage::width() const
{
    return mWidth;
}

int SoilImage::height() const
{
    return mHeight;
}
