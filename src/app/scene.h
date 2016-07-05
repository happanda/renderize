#pragma once
#include "camera/camera.h"


struct Scene
{
    template <class Number>
    void resize(glm::tvec2<Number> const& size)
    {
        resize(glm::vec2(static_cast<float>(size.x), static_cast<float>(size.y)));
    }

    void update(float dt);
    void draw();

private:
    void resize(glm::vec2 const& size);

    Camera mCamera;
};
