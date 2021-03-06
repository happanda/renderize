#pragma once
#include "buffers/texture.h"
#include "shaders/program.h"


struct Camera;
struct SoilImage;

struct Skybox
{
    Skybox();
    ~Skybox();
    Skybox(Skybox&& rhs);
    Skybox const& operator=(Skybox&& rhs);
    Skybox(Skybox const&) = delete;
    Skybox const& operator=(Skybox const&) = delete;

    void create(SoilCubemapImage const& imgs);
        /// draw before the scene, works with transparent objects
    void drawFirst(Camera const& camera);
        // draw after the scene, doesn't work with transparent objects
    void drawLast(Camera const& camera);

    Texture& tex();

private:
    void free();

    Texture mTex;
    Program mProg;
    GLuint mVAO{ 0 };
    GLuint mVBO{ 0 };
};
