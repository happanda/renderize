#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


struct Program;
struct UniformBuffer;

struct Camera
{
    template <class Number, glm::precision P = glm::defaultp>
    Camera(glm::tvec2<Number, P> const& size)
        : mSize(glm::vec2(static_cast<float>(size.x), static_cast<float>(size.y)))
    {
        setDefault();
    }

    glm::vec2 const& size() const;
    float near() const;
    float far() const;
    float fov() const;
    
    void size(glm::vec2 const& sz);
    void near(float val);
    void far(float val);
    void fov(float val);

    glm::mat4 projection() const;
    
    glm::vec3 const& pos() const;
    glm::vec3 const& front() const;
    glm::vec3 const& up() const;
    float speed() const;

    void pos(glm::vec3 const& v);
    void front(glm::vec3 const& v);
    void front(float pitch, float yaw);
    void up(glm::vec3 const& v);
    void speed(float speed);

    glm::mat4 view() const;

    void assign(Program& prog) const;
    void assign(UniformBuffer& uBuf) const;

private:
    void setDefault();

    glm::vec2 mSize;
    float mNear;
    float mFar;
    float mFov;
    
    glm::vec3 mPos;
    glm::vec3 mFront;
    glm::vec3 mUp;

    float mSpeed;
};
