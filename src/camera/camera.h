#pragma once
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>


struct camera
{
    camera(size_t width, size_t height);

    float near() const;
    float far() const;
    float fov() const;
    
    void near(float val);
    void far(float val);
    void fov(float val);

    glm::mat4 projection() const;
    
    glm::vec3 const& pos() const;
    glm::vec3 const& front() const;
    glm::vec3 const& up() const;

    void pos(glm::vec3 const& v);
    void front(glm::vec3 const& v);
    void up(glm::vec3 const& v);

    glm::mat4 view() const;

private:
    size_t mWidth;
    size_t mHeight;
    float mNear;
    float mFar;
    float mFov;
    
    glm::vec3 mPos;
    glm::vec3 mFront;
    glm::vec3 mUp;
};
